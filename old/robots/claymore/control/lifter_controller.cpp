#include "lifter_controller.h"
#include "message_logger.h"
#include "../subsystems.h"
#include <cmath>
#include <iostream>

using namespace std ;

//
// The nubmer of inches per tick for the lifter
//
const double LifterController::INCHES_PER_TICK_HIGH_GEAR = .08327 ;

//
// The height of the top of the collector in inches
//
const double LifterController::COLLECTOR_OFFSET = 11.375;


LifterController::LifterController()
{
    mMode = Mode::IDLE;
    mTarget = 11.375 ;
	mCurrent = 11.375 ;
	mLastHeight = 11.375 ;
    mLastTarget = 11.375 ;
	
    mHeightThreshold = 0.0;
    mLastVoltage = 0.0;
    mMaxChange = 6.0;
	
    mDataDumpMode = false;
    mDataDumpStartTime = 0.0;
	
    mCalibrated = false ;
	mClimbBaseRecorded = false ;
	mGear = Gear::HIGH ;
}

void LifterController::init()
{
	paramsInput *p = paramsInput::get() ;
    mHeightThreshold = p->getValue("lifter:threshold", 1.0);
}

void LifterController::moveToHeight(double height, double time, bool background)
{
	if (std::fabs(height - mCurrent) > mHeightThreshold)
	{
		mBackground = background;

		bool bNewReq = false ;
		double p, i, d, f, imax;
		double vmin, vmax ;
		paramsInput *params_p = paramsInput::get() ;
		double smallthresh = params_p->getValue("lifter:downsmall:threshold", 6.0) ;
		double delta = mCurrent - height ;

		if (mMode != Mode::HEIGHT || std::fabs(mTarget - height) > mHeightThreshold)
		{
			//
			// This is a new request, or our target has changed, restart our idea of
			// when the move request was initiated
			//
			mStartTime = time ;
			cout << "Lifter: Start Time Changed " << mStartTime << endl ;
			bNewReq = true ;
		}
		
		mMode = Mode::HEIGHT;
		mTarget = height;
		
		if(mCurrent < height)
		{
			p = params_p->getValue("lifter:up:p", 0.01);
			i = params_p->getValue("lifter:up:i", 0.0);
			d = params_p->getValue("lifter:up:d", 0.0);
			f = params_p->getValue("lifter:up:f", 0.0);
			imax = params_p->getValue("lifter:up:imax", 1000.0);
			vmin = params_p->getValue("lifter:up:vmin", -0.8) ;
			vmax = params_p->getValue("lifter:up:vmax", 0.8) ;
		}
		else if (mCurrent >= height && delta > smallthresh)
		{
			p = params_p->getValue("lifter:down:p", 0.01);
			i = params_p->getValue("lifter:down:i", 0.0);
			d = params_p->getValue("lifter:down:d", 0.0);
			f = params_p->getValue("lifter:down:f", 0.0);
			imax = params_p->getValue("lifter:down:imax", 1000.0);
			vmin = params_p->getValue("lifter:down:vmin", -0.8) ;
			vmax = params_p->getValue("lifter:down:vmax", 0.8) ;
		}
		else
		{
			p = params_p->getValue("lifter:downsmall:p", 0.01);
			i = params_p->getValue("lifter:downsmall:i", 0.0);
			d = params_p->getValue("lifter:downsmall:d", 0.0);
			f = params_p->getValue("lifter:downsmall:f", 0.0);
			imax = params_p->getValue("lifter:downsmall:imax", 1000.0);
			vmin = params_p->getValue("lifter:downsmall:vmin", -0.8) ;
			vmax = params_p->getValue("lifter:downsmall:vmax", 0.8) ;
		}
			
		mHeightPID.Init(p, i, d, f, vmin, vmax, imax);

		if (bNewReq)
		{
			messageLogger &logger = messageLogger::get();
			logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_LIFTER_TUNING);
			logger << "moveToHeight" ;
			logger << ", current " << mCurrent ;
			logger << ", target " << height;
			logger << ", pid " << p << " " << i << " " << d << " " << f << " " << imax;
			logger << ", vmin " << vmin << ", vmax " << vmax ;
			logger.endMessage();
		}
	}
	else
	{
		messageLogger &logger = messageLogger::get();
		logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_LIFTER);
		logger << "moveToHeight, target = " << height ;
		logger << ", current = " << mCurrent ;
		logger << ", threshold = " << mHeightThreshold ;
		logger << ", already at requested height" ;
		logger.endMessage();
	}
}

void LifterController::moveToHeight(Preset preset, double time, bool background)
{
    moveToHeight(presetToHeight(preset), time, background);
}

void LifterController::updateIdle(double time, double dt, double &out, Gear &gear, bool &brake)
{
	paramsInput *params_p = paramsInput::get() ;
	messageLogger &logger = messageLogger::get();

	logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_LIFTER);
	logger << "Lifter: State is IDLE" ;

	if (params_p->getValue("lifter:idle:use_brake", 0.0) > 0.5)
	{
		out = 0.0 ;
		gear = mGear ;
		brake = true ;
		logger << ", applying brake" ;
	}
	else
	{
		if (mGear == Gear::LOW)
			out = params_p->getValue("lifter:hold_power:low_gear", 0.0) ;
		else
			out = params_p->getValue("lifter:hold_power:high_gear", 0.1) ;
		
		gear = mGear ;
		logger << ", applying holding voltage" ;
	}
	logger.endMessage() ;
	
    if (mDataDumpMode) {
		logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_LIFTER_TUNING);
		logger << "lifter MODE idle, dt " << dt;
		logger << ", time " << time;
		logger << ", height " << mCurrent ;
		logger << ", ticks " << mTicks ;
		logger.endMessage();
		
		if (time - mDataDumpStartTime > 5.0)
			mDataDumpMode = false;
    }
}

//
// updateCalibrate just runs for a single robot loop and we record the number of ticks
// that we see a the bottom of the lift travel.  This should only be called when
// the lift is at the bottom of travel.
//
void LifterController::updateCalibrate(double time, double dt, double &out, Gear &gear, bool &brake)
{
	out = 0.0 ;
	gear = mGear ;

	mBaseTicks = mTicks ;
	mCalibrated = true ;

	calcHeight() ;
	mLastHeight = mCurrent ;

	mMode = Mode::IDLE ;
	
	messageLogger &logger = messageLogger::get();
	logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_LIFTER);
	logger << "Lifter: State is CALIBRATE, baseticks " << mBaseTicks ;
	logger.endMessage() ;
}

//
// updateHeight is called when the mode is HEIGHT mode.  It drives the lift up or down
// as necessare to move the lift to the target height
//
void LifterController::updateHeight(double time, double dt, double &out, Gear &gear, bool &brake)
{
	messageLogger &logger = messageLogger::get();

	//
	// If the lift is not calibreated, we cannot trust heights so we don't move to
	// heights
	//
	if (mCalibrated)
	{
		if (std::fabs(mTarget - mCurrent) < mHeightThreshold)
		{
			double elapsed= time - mStartTime ;
			logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_LIFTER_TUNING);
			logger << "Lifter: success, " ;
			logger << "current " << time ;
			logger << ", elapsed = " << elapsed << " secs" ;
			logger << ", delta " << mTarget - mCurrent ;
			logger << ", threshold " << mHeightThreshold ;
			logger.endMessage() ;
			
			mMode = Mode::IDLE ;
			mBackground = false;
			updateIdle(time, dt, out, gear, brake) ;
			
			mDataDumpMode = true;
			mDataDumpStartTime = time;
		}
		else
		{
			//
			// Get the output from the PID controller
			//
			out = mHeightPID.getOutput(mTarget, mCurrent, dt);
			
			//
			// Don't left the motors change voltage to quickly to prevent any
			// kind of brown out
			//
			double chg = std::fabs(out - mLastVoltage);
			if (chg > mMaxChange * dt)
			{
				if (out > mLastVoltage)
					out = mLastVoltage + mMaxChange * dt;
				else
					out = mLastVoltage - mMaxChange * dt;
			}
			mLastVoltage = out;
			
			logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_LIFTER_TUNING);
			logger << "lifter mode HEIGHT";
			logger << ", time " << time;
			logger << ", dt "<< dt;
			logger << ", target " << mTarget;
			logger << ", height " << mCurrent ;
			logger << ", ticks " << mTicks ;
			logger << ", out " << out;
			logger.endMessage() ;
		}
	}
	else
	{
		logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_LIFTER);
		logger << "Lifter: State is HEIGHT, ignored, not calibrated" ;
		logger.endMessage() ;
	}
}

void LifterController::updateClimb(double time, double dt, double &out, Gear &gear, bool &brake)
{
	paramsInput *params_p = paramsInput::get() ;
	messageLogger &logger = messageLogger::get();
	
	out = params_p->getValue("lifter:climb_power", -0.6) ;
	gear = Gear::LOW ;
	mCalibrated = false ;
	
	int climb_height_ticks = params_p->getValue("lifter:climbing_difference", 100) ;
	
	logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_LIFTER);
	logger << "Lifter: State is CLIMB" ;
	logger << ", ticks " << mTicks ;
	logger << ", climbbase " << mClimbBase ;
	logger << ", heightticks " << climb_height_ticks ;
		
	if (mTicks < mClimbBase - climb_height_ticks)
	{
		logger << "\nGOING INTO MAINTAIN";

		//
		// We have reached our climb goal, move to the maintain state
		//
		mMode = Mode::MAINTAIN ;
	}
	logger.endMessage() ;
}

void LifterController::updateMaintain(double time, double dt, double &out, Gear &gear, bool &brake)
{
	paramsInput *params_p = paramsInput::get() ;
	messageLogger &logger = messageLogger::get();
	
	int climb_height_ticks = params_p->getValue("lifter:climbing_difference", 100) ;
	int threshold = static_cast<int>(params_p->getValue("lifter:maintain_climb_threshold", 10)) ;
	
	logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_LIFTER);
	logger << "Lifter: State is MAINTAIN" ;
	logger << ", ticks " << mTicks ;
	logger << ", climbbase " << mClimbBase ;
	logger << ", heightticks " << climb_height_ticks ;
	logger << ", threshold " << threshold ;
	
	gear = Gear::LOW ;
	mCalibrated = false ;
	if (mTicks > mClimbBase - climb_height_ticks + threshold)
	{
	    brake = false ;
		out = params_p->getValue("lifter:climb_power", -0.6) ;
		logger << ", applying climb power" ;
	}
	else
	{
	    brake = true ;
		out = 0.0 ;
		logger << ", braking" ;
	}
	logger.endMessage() ;
}

void LifterController::updateUp(double time, double dt, double &out, Gear &gear, bool &brake)
{
	paramsInput *params_p = paramsInput::get() ;
	messageLogger &logger = messageLogger::get();
	
	double top_limit = params_p->getValue("lifter:height:top_limit", 96.0);

	double slowdown_range;
	if(mHighPower)
		slowdown_range = params_p->getValue("lifter:slowdown_range:high_power", 12.0);
	else
		slowdown_range = params_p->getValue("lifter:slowdown_range:low_power", 6.0);

	logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_LIFTER);
	logger << "Lifter: State is UP" ;
	
	gear = mGear ;
	brake = false ;
	
	if (!mCalibrated)
	{
		logger << ", uncalibrated" ;
		out = params_p->getValue("lifter:manual_power:low", 0.4) ;
		assert(out >= 0.0) ;
	}
	else if (mCurrent >= top_limit)
	{
		brake = true;
		out = 0.0 ;
		logger << ", top limit" ;
	}
	else if (mCurrent > top_limit - slowdown_range)
	{
		logger << ", upper slowdown" ;
		out = params_p->getValue("lifter:slowdown_power", 0.2) ;
		assert(out >= 0.0) ;
	}
	else if (mHighPower)
	{
		logger << ", high power" ;
		out = params_p->getValue("lifter:manual_power:high", 0.8) ;
		assert(out >= 0.0) ;
	}
	else
	{
		logger << ", low power" ;
		out = params_p->getValue("lifter:manual_power:low", 0.4) ;
		assert(out >= 0.0) ;
	}

	logger << ", out " << out ;
	logger.endMessage() ;

	mMode = Mode::IDLE ;
}

void LifterController::updateDown(double time, double dt, double &out, Gear &gear, bool &brake)
{
	paramsInput *params_p = paramsInput::get() ;
	messageLogger &logger = messageLogger::get();
	
    double bottom_limit = params_p->getValue("lifter:collector_offset", 11.375) ;
    
	double slowdown_range;
	if(mHighPower)
		slowdown_range = params_p->getValue("lifter:slowdown_range:high_power", 12.0);
	else
		slowdown_range = params_p->getValue("lifter:slowdown_range:low_power", 6.0);
	
	logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_LIFTER);
	logger << "Lifter: State is DOWN" ;
	
	gear = mGear ;
	brake = false ;
	if (!mCalibrated)
	{
		logger << ", uncalibrated" ;
		out = -params_p->getValue("lifter:manual_power:low", 0.4) ;
		assert(out <= 0.0) ;
	}
	/*else if (mCurrent <= bottom_limit)
	{
		brake = true;
		logger << ", bottom limit" ;
		out = 0.0 ;
	}*/
	else if (mCurrent < bottom_limit + slowdown_range)
	{
		logger << ", lower slowdown" ;
		out = -params_p->getValue("lifter:slowdown_power", 0.2) ;
		assert(out <= 0.0) ;
	}
	else if (mHighPower)
	{
		logger << ", high power" ;
		out = -params_p->getValue("lifter:manual_power:high", 0.8) ;
		assert(out <= 0.0) ;
	}
	else
	{
		logger << ", low power" ;
		out = -params_p->getValue("lifter:manual_power:low", 0.4) ;
		assert(out <= 0.0) ;
	}
	logger << ", out " << out ;
	logger.endMessage() ;
	
	mMode = Mode::IDLE ;
}

void LifterController::update(int ticks, bool ulimit, bool blimit, double time, double dt, double& out, Gear &gear, bool &brake)
{
	messageLogger &logger = messageLogger::get();
	
	//
	// Remember the current ticks number
	//
	mTicks = ticks ;
	
	//
	// Remember the previous height
	//
	mLastHeight = mCurrent ;

	//
	// Calculate the current height based on the ticks
	//
	calcHeight() ;

	//
	// Find the speed of the lift in inches per second
	//
	mSpeed = (mCurrent - mLastHeight) / dt ;

	//
	// Default state of the lifter is do nothing
	//
	out = 0.0 ;
	gear = mGear ;
	brake = false ;
	
	switch(mMode)
	{
	case Mode::IDLE:
		updateIdle(time, dt, out, gear, brake) ;
		break ;

	case Mode::CALIBRATE:
		updateCalibrate(time, dt, out, gear, brake) ;
		break ;

	case Mode::HEIGHT:
		updateHeight(time, dt, out, gear, brake) ;
		break ;

	case Mode::CLIMB:
		updateClimb(time, dt, out, gear, brake) ;
		break ;

	case Mode::MAINTAIN:
		updateMaintain(time, dt, out, gear, brake) ;
		break ;

	case Mode::UP:
		updateUp(time, dt, out, gear, brake) ;
		break;

	case Mode::DOWN:
		updateDown(time, dt, out, gear, brake) ;
		break ;
	}

	//
	// If the limit switch is on and we are continuing to move in the wrong
	// direction, we shut it down here
	//
	if ((ulimit && out > 0.0) || (blimit && out < 0.0)) {
		brake = true;
		out = 0.0 ;
	}

	logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_LIFTER);
	logger << "Lifter Output:" ;
	logger << ", out " << out ;
	logger << ", gear " << (gear == Gear::LOW ? "LOW" : "HIGH") ;
	logger << ", brake " << (brake ? "ON" : "OFF") << "\n" ;
	logger << " Lifter Height: " << mCurrent << "\n";
	logger.endMessage() ;
}

void LifterController::resetCalibration() {
	mCalibrated = false;
}

double LifterController::presetToHeight(Preset preset) const
{
	double ret = 11.375 ;
	paramsInput *params_p = paramsInput::get() ;
	
	
    switch(preset) {
    case Preset::FLOOR:
		ret = params_p->getValue("lifter:height:floor", 0.0);
		break ;
		
    case Preset::EXCHANGE:
		ret = params_p->getValue("lifter:height:exchange", 3.0);
		break ;
		
    case Preset::DROP_GRABBER:
		ret = params_p->getValue("lifter:height:drop_grabber", 6.0);
		break ;
		
    case Preset::SWITCH:
		ret = params_p->getValue("lifter:height:switch", 19.0);
		break ;
		
    case Preset::SCALE:
		ret = params_p->getValue("lifter:height:scale", 72.0);
		break ;
		
    case Preset::SCALE_HIGH:
		ret = params_p->getValue("lifter:height:scale_high", 72.0);
		break ;
		
    case Preset::PREP_CLIMB:
		ret = params_p->getValue("lifter:height:prep_climb", 84.0);
		break ;
    }

	return ret ;
}
