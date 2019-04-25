#include "grabber_controller.h"
#include "message_logger.h"
#include "../subsystems.h"
#include <cmath>
#include <iostream>

using namespace std ;

GrabberController::GrabberController()
{
    mCubeState = CubeState::NoCube ;
    mPrevCubeState = CubeState::NoCube ;
    mArmState = ArmState::IDLE ;
	mPrevArmState = ArmState::IDLE ;
    mTarget = 0.0;
    mLastTarget = 0.0;
    mAngleThreshold = 0.0;
    mLastVoltage = 0.0;
    mMaxChange = 6.0;
    mDataDumpMode = false;
    mDataDumpStartTime = 0.0;
    
}

void GrabberController::init()
{
    paramsInput *params_p = paramsInput::get() ;
	
    mAngleThreshold = params_p->getValue("grabber:threshold", 1.0);
    mMaximumAngle = params_p->getValue("grabber:angle:maximum", 90.0) ;
    mMinimumAngle = params_p->getValue("grabber:angle:minimum", 0.0) ;
    mStowedAngle = params_p->getValue("grabber:angle:stowed", 90.0) ;
    mCloseCollectAngle = params_p->getValue("grabber:angle:closed", 0.0);
    mOpenCollectAngle = params_p->getValue("grabber:angle:open", 30.0);
}


void GrabberController::moveToAngle(double angle, double time)
{
    paramsInput *params_p = paramsInput::get() ;

	setState(ArmState::ANGLE) ;
	mTarget = angle;
	mStartTime = time ;

	double p = params_p->getValue("grabber:p", 0.01);
	double i = params_p->getValue("grabber:i", 0.0);
	double d = params_p->getValue("grabber:d", 0.0);
	double f = params_p->getValue("grabber:f", 0.0);
	double vmin = params_p->getValue("grabber:vmin", -0.4);
	double vmax = params_p->getValue("grabber:vmax", 0.4);
	double imax = params_p->getValue("grabber:imax", 1000.0);
	
	mAnglePID.Init(p, i, d, f, vmin, vmax, imax);
	
	messageLogger &logger = messageLogger::get();
	logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_GRABBER_TUNING);
	logger << "moveToAngle, angle = " << angle;
	logger << ", pid " << p << " " << i << " " << d << " " << f ;
	logger << "vmin " << vmin << " vmax " << vmax << " " << imax;
	logger.endMessage();
}

void GrabberController::moveToAngle(Preset preset, double time)
{
    moveToAngle(presetToAngle(preset), time);
}


//
// Note, this is a state machine
//
// States:
//
// NoCube        - the collector is not holding a cube
// MaybeHasCube  - the collector may be holding a cube but we want the has_cube
//                 signal to stay active long enough to be sure (250 ms)
// HasCube       - the collector is holding a cube
// MaybeLostCube - the collector was previously holding a cube and may have lost it
//                 we want the has_cube signal to stay inactive long enough to be
//                 sure
//
void GrabberController::processCubeState(bool cubesensor, bool arms_in, double now)
{
	paramsInput* params = paramsInput::get();
	double delay ;

	messageLogger &logger = messageLogger::get();
	logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_GRABBER);
	logger << "time: " << now << "\n";
	logger << "Cube state: ";
	logger << (int) mCubeState;
	logger.endMessage();
	
    mPrevCubeState = mCubeState;
    switch(mCubeState)
    {
    case CubeState::NoCube:
		if (cubesensor && !arms_in)
		{
			//
			// We are currently in the no cube state, but see the cube
			// present signal.  We start a timer to be sure the cube
			// sensor signal sticks around
			//
			mCubeState = CubeState::MaybeHasCube ;
			delay = params->getValue("grabber:maybecube", 0.25) ;
			mCubeStateTimer.set(delay) ;
		}
		break ;

    case CubeState::MaybeHasCube:
		if (!cubesensor)
		{
			//
			// The has cube disappeared before the timer expired.  We assume
			// this was a glitch and do not respond to the cube
			//
			mCubeState = CubeState::NoCube ;
		}
		else if (mCubeStateTimer.done())
		{
			//
			// The timer for the has cube status has expired, assume we really
			// have a cube.
			//
			mCubeState = CubeState::GraspCube ;
			mArmState = ArmState::CLAMP ;
			delay = params->getValue("grabber:graspingcube", 0.5) ;
			mCubeStateTimer.set(delay) ;
		}
		break ;

	case CubeState::GraspCube:
		//
		// TODO - add code to deal with losing the cube detect sensor
		//

		if (std::fabs(mAngle - mCloseCollectAngle) < mAngleThreshold)
		{
			mCubeState = CubeState::HasCube ;
			mArmState = ArmState::HOLD ;
		}
		else if (mCubeStateTimer.done())
		{
			//
			// We waited for the grasper to close, but it has not, just assume we
			// are ok
			//
			mCubeState = CubeState::HasCube ;
			mArmState = ArmState::HOLD ;
		}
		break ;
		
    case CubeState::HasCube:
		if (!cubesensor)
		{
			//
			// We lost the has cube signal, so we may have lost the cube, or the
			// signal may have just disappeared temporarily.  Start a timer to see if the
			// cube is really gone
			//
			mCubeStateTimer.set(params->getValue("grabber:maybelostcube", 0.5)) ;
			mCubeState = CubeState::MaybeLostCube ;
		}
		break ;
    case CubeState::MaybeLostCube:
		if (cubesensor)
		{
			//
			// The cube has reappeared, just assume some flakiness with the sensor or the
			// the cube is jostling around.
			//
			mCubeState = CubeState::HasCube ;
		}
		else if (mCubeStateTimer.done())
		{
			//
			// The cube is really gone, go back to the no cube state
			//
			mCubeState = CubeState::NoCube ;
		}
		break ;
    }
    
    mCubeStateTimer.update(now, true);
}

void GrabberController::update(double angle, bool cubesensor, double time, double dt, double& out)
{
	messageLogger &logger = messageLogger::get();
	paramsInput *params_p = paramsInput::get() ;

	mAngle = angle ;

	if (mPrevArmState != mArmState)
	{
		logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_GRABBER);
		logger << "Switched state: " ;
		logger << armStateToString(mPrevArmState) ;
		logger << " -> " ;
		logger << armStateToString(mArmState) ;
		logger.endMessage() ;
	}

    //
    // Update the cube state
    //
    processCubeState(cubesensor, angle < presetToAngle(Preset::CLOSED), time) ;

    switch(mArmState)
    {
    case ArmState::CALIBRATING:
		calibrate(angle, dt, out) ;
		break ;

    case ArmState::IDLE:
		idleState(angle, time, dt) ;
		out = 0.0 ;
		break ;

    case ArmState::HOLD:
		out = params_p->getValue("grabber:hold_power", -0.1) ;
		break ;

	case ArmState::CLAMP:
		out = params_p->getValue("grabber:clamp_power", -0.3) ;
		break;

    case ArmState::OPEN:
		out = params_p->getValue("grabber:manual_power", 0.2) ;
		break ;

    case ArmState::CLOSE:
		out = -params_p->getValue("grabber:manual_power", 0.2) ;
		break ;

    case ArmState::ANGLE:
		angleState(angle, time, dt, out) ;
		break ;
    }

    if (mCalibrated)
    {
		//
		// If we have calibrated, and the grabber to opening too wide or
		// closing too much, we shut it down
		//
		if (angle >= mStowedAngle && out > 0.0)
			out = 0.0 ;
		else if (angle <= mMinimumAngle && out < 0.0)
			out = 0.0 ;
    }

	mPrevArmState = mArmState ;
}

void GrabberController::calibrate(double angle, double dt, double &out)
{
	out = -0.4 ;
}

void GrabberController::resetCalibration()
{
	mCalibrated = false;
}

void GrabberController::angleState(double angle, double time, double dt, double &out)
{
    messageLogger &logger = messageLogger::get();
    if(std::fabs(mTarget - angle) < mAngleThreshold)
    {
		mDataDumpMode = true;
		mDataDumpStartTime = time;
		mArmState = ArmState::IDLE ;
		out = 0.0 ;
	
		logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_GRABBER_TUNING);
		double elapsed= time - mStartTime ;
		logger << "Grabber Reached Goal (SUCCESS) " ;
		logger << elapsed << " seconds" ;
		logger << ", target " << mTarget ;
		logger << ", angle " << angle ;
		logger.endMessage() ;
    }
    else
    {
		out = mAnglePID.getOutput(mTarget, angle, dt);
	
		double chg = std::fabs(out - mLastVoltage);
		if (chg > mMaxChange * dt) {
			if (out > mLastVoltage)
				out = mLastVoltage + mMaxChange * dt;
			else
				out = mLastVoltage - mMaxChange * dt;
		}
		mLastVoltage = out;
	
		logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_GRABBER_TUNING);
		logger << "grabber:update(ANGLE)";
		logger << ", time " << time;
		logger << ", dt "<< dt;
		logger << ", target " << mTarget;
		logger << ", angle " << angle;
		logger << ", out " << out;
		logger.endMessage() ;
    }
}

void GrabberController::idleState(double angle, double time, double dt) {
    if (mDataDumpMode) {
		messageLogger &logger = messageLogger::get();
		logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_GRABBER_TUNING);
		logger << "IDLE: dt " << dt;
		logger << ", time " << time;
		logger << ", angle " << angle;
		logger.endMessage();
	
		if (time - mDataDumpStartTime > 5.0)
			mDataDumpMode = false;
    }
}
bool GrabberController::done()
{
    return mMode == Mode::IDLE;
}

double GrabberController::presetToAngle(Preset preset)
{

    switch(preset) {
    case Preset::CLOSED: return mCloseCollectAngle;
    case Preset::OPEN: return mOpenCollectAngle;
    case Preset::STOWED: return mStowedAngle;
    default: assert(0);
    }
}
