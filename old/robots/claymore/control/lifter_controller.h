#ifndef LIFTER_CONTROLLER_H
#define LIFTER_CONTROLLER_H

#include "pidctrl.h"
#include "params_parser.h"
#include <cmath>
#include <iostream>

//
//

class LifterController {
public:
	/// \brief the inches per tick for the lifter
	static const double INCHES_PER_TICK_HIGH_GEAR ;

	/// \brief the offset to the top of the collector when at its lowest height
	static const double COLLECTOR_OFFSET ;
	
    /// \brief the presets that the lifter already knows about.
    enum class Preset {
		FLOOR,				///< Move the lifter to the floor
		EXCHANGE,			///< Move the lifter to the exchange
		DROP_GRABBER,       ///< Move the lifter to a point where the grabber drops down
		SWITCH,				///< Move the lifter to the height of the switch
		SCALE,				///< Move the lifter to the height of the scale
		SCALE_HIGH,			///< Move the lifter to the high scale position ready to shoot cube
		PREP_CLIMB			///< Move the lifter to the climb position
    };

	enum class Gear {
		LOW,
		HIGH
	} ;

    /// \brief create the lift controller object
    LifterController();

	/// \brief initialize the lifter
	void init() ;

	bool isCalibrated() const
	{
		return mCalibrated ;
	}

	/// \brief return the climbed state
	bool isClimbed() const
	{
		return mMode == Mode::MAINTAIN ;
	}

    /// \brief move the lifter to a specific height
    /// \param height the height in inches for the lifter
    /// \param the start time of this operation
    void moveToHeight(double height, double time, bool background=false);

    /// \brief move the lifter to a preset position
    /// \param preset the preset for the lifter height
    /// \param time the start time of this operation
    void moveToHeight(Preset preset, double time, bool background=false);

    /// \brief this method is called each time the robot loop is run to update the lifer
    /// This method uses a PID controller to position the lifter to the desired location
	/// \param ticks the number of ticks at the encoder
	/// \param ulimit the state of the upper limit switch
	/// \param blimit the state of the lower limit switch
    /// \param time the current time in seconds
    /// \param dt the time that has elapsed since the last time this was called
    /// \param out the output voltage for the lifter motor
	/// \param the gear the lifter should be using
	/// \param the state of the break, true is on
    void update(int ticks, bool ulimit, bool blimit, double time, double dt, double &out, Gear &gear, bool &brake);

	/// \brief reset the controller to an uncalibrated state
	void resetCalibration();

    /// \brief returns true if the lifter has reached the specified height
    /// \param height the height to check against
    /// \returns true if the lifter has reached the specified height
    bool atHeight(double height) const
	{
		return mMode == Mode::IDLE && std::fabs(mCurrent -  height) < mHeightThreshold ;
	}
	
    /// \brief returns true if the lifter has reached the specified height
    /// \param preset the height to check against
    /// \returns true if the lifter has reached the specified height
    bool atHeight(Preset preset) const
	{
		return atHeight(presetToHeight(preset)) ;
	}

	/// \brief returns the height of the lifter
	double getHeight() const
	{
		return mCurrent ;
	}		

    /// \brief returns true when the lifter has reached its desired height
    /// \returns true when the lifter has reached its desired height
    bool done();

    /// \brief returns the height assocaited with a preset
    /// \param preset the preset of interest
    double presetToHeight(Preset preset) const ;

	/// \brief set lifter to idle state
	void idle()
	{
		if(!mBackground) 
			mMode = Mode::IDLE ;
	}

	/// \brief reset the climb base recorded flag
	void resetClimbBase()
	{
		mClimbBaseRecorded = false ;
	}

	/// \brief set the lifter in the climb mode
	void climb()
	{
		//
		// If we are moving into the climb state, remember the number of ticks we are
		// at as a reference for ending the climb
		//
		if (!mClimbBaseRecorded)
		{
			mClimbBase = mTicks ;
			mClimbBaseRecorded = true ;
		}
		
		mMode = Mode::CLIMB ;
	}

	/// \brief set the lifter to maintain the current height
	void maintain()
	{
		mMode = Mode::MAINTAIN ;
	}

	void calibrate()
	{
		mMode = Mode::CALIBRATE ;
	}

	void up(bool highpower)
	{
		mBackground = false;
		mMode = Mode::UP ;
		mHighPower = highpower ;
	}

	void down(bool highpower)
	{
		mBackground = false;
		mMode = Mode::DOWN ;
		mHighPower = highpower ;
	}

	void lowgear()
	{
		std::cout << "Set to low gear" << std::endl ;
		mGear = Gear::LOW ;
		mCalibrated = false ;
	}

	void highgear()
	{
		mGear = Gear::HIGH ;
	}

private:
	void updateIdle(double time, double dt, double &out, Gear &gear, bool &brake);
	void updateCalibrate(double time, double dt, double &out, Gear &gear, bool &brake);
	void updateHeight(double time, double dt, double &out, Gear &gear, bool &brake);
	void updateClimb(double time, double dt, double &out, Gear &gear, bool &brake);
	void updateMaintain(double time, double dt, double &out, Gear &gear, bool &brake);
	void updateUp(double time, double dt, double &out, Gear &gear, bool &brake);
	void updateDown(double time, double dt, double &out, Gear &gear, bool &brake);
	void updateLock(double time, double dt, double &out, Gear &gear, bool &brake);

	void calcHeight()
	{
		mCurrent = (mTicks - mBaseTicks) * INCHES_PER_TICK_HIGH_GEAR + COLLECTOR_OFFSET ;
	}

private:
    // Indicates the mode of the lifter
    enum class Mode {
		IDLE,			// Doing nothing
		CALIBRATE,		// Calibrate the lifter
		HEIGHT,			// Seeking a desired height
		CLIMB,			// The lifter is climbing
		MAINTAIN,		// The lifter is maintaining climb height
		UP,				// Move the lifter up
		DOWN,			// Move the lifter down
    };

    //
    // The current mode for the lifter
    //
    Mode mMode;

	//
	// The gear we are currently in
	//
	Gear mGear ;

	//
	// If true, perform manual operations on the lift in high power
	//
	bool mHighPower ;

    //
    // The target height
    //
    double mTarget;

	//
	// The current height
	//
	double mCurrent ;

	//
	// The last height, used to calculate speed
	//
	double mLastHeight ;

	//
	// The speed of the lift in inches per second up or down
	//
	double mSpeed ;

	//
	// The current number of ticks
	//
	int mTicks ;

	//
	// The number of ticks when the lifter is at the bottom
	//
	int mBaseTicks ;

	//
	// The target ticks for climbing
	//
	int mClimbBase ;

    //
    // The last target value that was applied
    //
    double mLastTarget;

    //
    // The threshold we use to consider a height target met
    //
    double mHeightThreshold;

    //
    // The start time of a height seeking operation
    //
    double mStartTime ;

    //
    // The PID controller for seeking a given height
    //
    PIDCtrl mHeightPID;

    //
    // The last voltage returned for the motor
    //
    double mLastVoltage;

    //
    // The maximum change in volts per second for the motor voltage
    //
    double mMaxChange;

    //
    // If true, we have finished a height operation and we are continuing to
    // dump data about the lifter height.  This gives us the ability to track
    // any drift we may see after the operation is complete.
    bool mDataDumpMode;

    //
    // The start time for dumping data.  We only dump data after an operation for
    // a fixed amount of time (e.g. 5 seconds).
    //
    double mDataDumpStartTime;

    //
    // Represents whether or not the lifter is calibrating
    //
    bool mCalibrating;

    //
    // if true, we hvae seen the mCalibrating go from true to false indicating
    // that the lifter has been calibrated
    //
    bool mCalibrated ;

	//
	// If true, our base for climbing has been recorded
	//
	bool mClimbBaseRecorded ;
	
	bool mBackground;
};

#endif
