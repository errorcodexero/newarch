#ifndef GRABBER_CONTROLLER_H
#define GRABBER_CONTROLLER_H

#include "pidctrl.h"
#include "params_parser.h"
#include "countdown_timer.h"
#include <iostream>

class GrabberController {
public:
    /// \brief the presets that the grabber already knows about.
    enum class Preset {
		CLOSED,				///< Move the grabber to the 'closed' position
		OPEN,                           ///< Move the grabber to the 'open' position
		STOWED                          ///< Move the grabber to the 'stowed' position
	};

    enum class CubeState {
		NoCube,
		MaybeHasCube,
		GraspCube,
		HasCube,
		MaybeLostCube,
	} ;

    enum class ArmState {
		CALIBRATING,	       ///< Calibrating the grabber to establish position reference
		IDLE,			       ///< Let the grabber be idle
		HOLD,			       ///< Hold a cube in the grabber
		CLAMP,                 ///< Clamp down after collecting a cube
		ANGLE,			       ///< Move the grabber to a position
		OPEN,			       ///< Manually open the grabber arm
		CLOSE,			       ///< Manually close the grabber arm
	} ;

    /// \brief create the grabber controller object
    GrabberController();

	/// \brief initialize the grabber controller
	void init() ;

    /// \brief return the cube state for the grabber
    CubeState getCubeState()
    {
		return mCubeState ;
    }

    /// \brief returnt he ARM state for the grabber
    ArmState getArmState()
    {
		return mArmState ;
    }

    /// \brief set the arm state to idle
    void setIdle()
    {
		setState(ArmState::IDLE) ;
    }
	
    void hold()
    {
		setState(ArmState::HOLD)  ;
    }

	void clamp()
	{
		setState(ArmState::CLAMP) ;
	}

    void open()
    {
		setState(ArmState::OPEN) ;
    }

    void close()
    {
		setState(ArmState::CLOSE) ;
    }

    void calibrate()
    {
		setState(ArmState::CALIBRATING) ;
    }

    /// \brief move the grabber to a specific angle
    /// \param angle the angle in degrees for the grabber
    /// \param the start time of this operation
    void moveToAngle(double angle, double time);

    /// \brief move the grabber to a preset position
    /// \param preset the preset for the grabber angle
    /// \param time the start time of this operation
    void moveToAngle(Preset preset, double time);

    /// \brief this method is called each time the robot loop is run to update the grabber
    /// This method uses a PID controller to position the grabber at the desired angle
    /// \param angle the current angle of the grabber
    /// \param cubesensor the state of the cube sensor
    /// \param time the current time in seconds
    /// \param dt the time that has elapsed since the last time this was called
    /// \param out the output voltage for the grabber motor
    void update(double angle, bool cubesensor, double time, double dt, double &out);

	/// \brief reset the controller to an uncalibrated state
    void resetCalibration();

    /// \brief returns true when the grabber has reached its desired angle
    /// \returns true when the grabber has reached its desired angle
    bool done();

    /// \brief sets mDoneCalibrating to the parameter
    /// \param done whether the grabber has finished calibrating
    bool isCalibrating()
    {
		return mArmState == ArmState::CALIBRATING ;
    }

    /// \brief returns the angle associated with a preset
    /// \param preset the preset of interest
    double presetToAngle(Preset preset);

    /// \brief check for transitions of the cube state
    bool enterState(CubeState current)
    {
		return mPrevCubeState != current && mCubeState == current ;
    }

private:
	void setState(ArmState st)
	{
		mArmState = st ;
	}
	
    void processCubeState(bool cubesensor, bool arms_in, double now) ;
    void idleState(double angle, double time, double dt) ;
    void calibrate(double angle, double dt, double &out) ;
    void angleState(double angle, double time, double dt, double &out) ;

	std::string armStateToString(ArmState st)
	{
		std::string result="BADWOLF" ;
		
		switch(st)
		{
		case ArmState::CALIBRATING:
			result = "CALIBRATING" ;
			break ;
		case ArmState::IDLE:
			result = "IDLE" ;
			break ;
		case ArmState::HOLD:
			result = "HOLD" ;
			break ;
		case ArmState::CLAMP:
			result = "CLAMP" ;
			break;
		case ArmState::ANGLE:
			result = "ANGLE" ;
			break ;
		case ArmState::OPEN:
			result = "OPEN" ;
			break ;
		case ArmState::CLOSE:
			result = "CLOSE" ;
			break ;
		}

		return result ;
	}

private:
    // Indicates the mode of the grabber
    enum class Mode {
		IDLE,			// Doing nothing
		ANGLE			// Seeking a desired angle
	};

    //
    // The current mode for the grabber
    //
    Mode mMode;

    //
    // The target angle
    //
    double mTarget;

	//
	// THe last angle for the grabber
	//
	double mAngle ;

    //
    // The last target value that was applied
    //
    double mLastTarget;

    //
    // The threshold we use to consider an angle target met
    //
    double mAngleThreshold;

    //
    // The start time of an angle seeking operation
    //
    double mStartTime ;

    //
    // The PID controller for seeking a given angle
    //
    PIDCtrl mAnglePID;

    //
    // The global input params object
    //
    paramsInput* mInputParams;

    //
    // The last voltage returned for the motor
    //
    double mLastVoltage;

    //
    // The maximum change in volts per second for the motor voltage
    //
    double mMaxChange;

    //
    // If true, we have finished an angle operation and we are continuing to
    // dump data about the grabber angle.  This gives us the ability to track
    // any drift we may see after the operation is complete.
    bool mDataDumpMode;

    //
    // The start time for dumping data.  We only dump data after an operation for
    // a fixed amount of time (e.g. 5 seconds).
    //
    double mDataDumpStartTime;

    //
    // Represents whether or not the grabber has finished calibration
    //
    bool mCalibrated ;

    //
    // The state of the cube in the grabber
    //
    CubeState mCubeState ;

    //
    // The previous cube state
    //
    CubeState mPrevCubeState ;

    //
    // The state of the grabber
    //
    ArmState mArmState ;

	//
	// The previous state of the ARM
	//
	ArmState mPrevArmState ;

    //
    // The timer for the cube state
    //
    Countdown_timer mCubeStateTimer ;

    //
    // The angle for the grabber stowed
    //
    double mMaximumAngle ;

    //
    // The angle for the grabber at its maximum closed state
    //
    double mMinimumAngle ;

    //
    // The angle for close collecting
    //
    double mCloseCollectAngle ;

    //
    // The angle for open collecting
    //
    double mOpenCollectAngle ;

    //
    // The angle for stowing the grabber for climbing
    //
    double mStowedAngle ;
};

#endif
