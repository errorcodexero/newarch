#ifndef DRIVEBASE_CONTROLLER_H
#define DRIVEBASE_CONTROLLER_H

#include "pidctrl.h"
#include "params_parser.h"
#include "UdpBroadcastSender.h"
#include "subsystems.h"
#include <list>
#include <string>

/// \brief The DrivebaseController class contains the intelligence for the drivebase.
/// This class contains the algorithms to drive specific distances or to rotate to
/// specific angles.  This work is done with a series of PID controllers.
class DrivebaseController {
public:

    /// \brief create the drive base controller object
    DrivebaseController();

    /// \brief set the parameters object
    /// This object is used to lookup parameters read from the parameters file
    /// \param pi_p pointer to the parameter input reader
    void setParams(paramsInput *pi_p);

    /// \brief get the params object used to extract parameters from the params file
    /// \returns the params object
    paramsInput* getParams();

    /// \brief initialize the drive controller to drive a fixed distance value.
    /// It is assumed that the update method below will be called within the robot
    /// loop to drive this fixed distance.  This method initializes the internal state
    /// of the robot controller so that the update method knows what it is trying to
    /// accomplish.
    /// \param dist the absolute distance to drive to
    /// \param angle the angle to maintain while driving
    /// \param the time when this was requested
    void initDistance(double dist, double angle, double time, bool end_on_stall, bool forward, bool valet = false);
	
	void initCurve(double current_dist, double target_dist, double curve_start, double current_angle, double target_angle_offset, double time, bool end_on_stall, bool forward);

    /// \brief rotate to the given angle
    /// \param angle the angle to rotate to
    /// \param the time when this was requested
	/// \param posangle if true, the desired angle is a positive angle
    void initAngle(double angle, double time, bool posangle);
	
    /// \brief rotate to the given angle
    /// \param angle the angle to rotate to
    /// \param the time when this was requested
	/// \param posangle if true, the desired angle is a positive angle
	/// \param tol the tolerance of the turn
    void initAngle(double angle, double time, bool posangle, double tol);

    /// \brief update the left and right motor values to acheive the desired goal
    /// \param dl the distance traveled by the left wheel
    /// \param dr the distance traveled by the right wheel
    /// \param angle the current angle of the robot
    /// \param dt the time elapsed since the last time update was called
    /// \param time the absolute time value
    /// \param out_l return value for the left motor voltage
    /// \param out_r return value for the right motor voltage
    /// \param out_high_gear return value for whether to put the drivebase into high gear
    void update(double dl, double dr, double angle, double dt, double time, double &out_l, double &out_r, bool &out_high_gear);

    /// \brief called when the drivebase is idle
    /// \param dl the distance traveled by the left wheel
    /// \param dr the distance traveled by the right wheel
    /// \param angle the current angle of the robot
    /// \param dt the time elapsed since the last time update was called
    /// \param time the absolute time value
    void idle(double dl, double dr, double angle, double dt, double time);

	void abort()
	{
		mAbort = true ;
	}

    /// \brief returns true when the robot has reached its distance or angle goal
    bool done();

	/// \brief returns the actual last angle rotation
	double getLastAngle()
	{
		return mAngleLastActual ;
	}
	
private:
    enum class Mode {
		IDLE,
	    DISTANCE,
		CURVE,
	    ANGLE
	};

    //
    // The current mode of the drivebase.  Either trying to acheive a distance, or an
    // angle, or IDLE mode.  In IDLE mode the drivebase motors are controlled by the
    // drivebase.cpp code.
    //
    Mode mMode;

    //
    // The target distance or angle, depending on the mode of the drivebase_controller
    //
    double mTarget ;

	//
	// If true, we are driving straight forward (increasing distance)
	//
	bool mForward ;

	//
	// If true, abort the drive on the next update
	//
	bool mAbort ;

	//
	// if true, we are in valet mode, you cannot shift into high gear and go fast
	//
	bool mValet ;

    //
    // The time we started our search for the target distance or angle
    //
    double mTargetStartTime ;

    //
    // The starting angle, that we are trying to maintain, while driving straight
    //
    double mTargetCorrectionAngle;

	double mTargetCurveAngleOffset;

	double mInitialDistance;

	double mCurveStart ;

    //
    // The threshold for declaring a distance target met.
    //
    double mDistanceThreshold;

    //
    // The threshold for declaring an angle target met, from the params file and used
	// by default
    //
    double mAngleThreshold ;

	bool mAngleStartCaptured ;
	double mAngleStart ;

	//
	// The threshold for declaring the current angle target met.
	//
	double mAngleCurrentThreshold ;

	//
	// The actual angle from the last rotate operation
	//
	double mAngleLastActual ;

    //
    // The velocity threshold for declaring an angle target met.  The rotational
    // velocity of the robot must be below this value to consider the angle target
    // met.
    //
    double mAngleVThreshold;

    //
    // The PID controller for adjusting to the left and right motor to drive straight
    //
    PIDCtrl mStraightnessPid ;

    //
    // The PID controller for driving a given distance
    //
    PIDCtrl mDistPid ;

    //
    // The PID controller for rotating to a given distance
    //
    PIDCtrl mAnglePid;

    //
    // The distance of the robot during the last robot loop.  Used to calculate
    // the angular velocity of the robot
    //
    double mLastDistance;

    //
    // The angle of the robot during the last robot loop.  Used to calculate
    // the angular velocity of the robot
    //
    double mLastAngle;

    //
    // The distance travelled by the robot for the last N robot loops.  This
    // is used to determine if the robot is stalled
    //
    std::list<double> mHistory;

    //
    // The number of samples to keep to determine if the robot is stalled
    //
    size_t mNsamples;

    //
    // True if the drivebase is stalled
    //
    bool mStalled;

    //
    // If true, act as though the drivebase has finished when it stalls
    //
    bool mEndOnStall;

    //
    // True if the drivebase should be in high gear
    //
    bool mHighGear;

    //
    // If true, ther PID controller for distance has been reset from the long distance
    // PID constants, to the local PID constants
    //
    bool mResetPid;

    //
    // The threshold below which, the drivebase is considered stalled and the
    // PID constants are reset to the local PID constants
    //
    double mPidResetThreshold;

    //
    // The input paramsters object.  Used to get all tunable parameters from a
    // file.
    //
    paramsInput* mInputParams;

    //
    // The last voltage applied to the motors, used to ensure we dont ramp the
    // voltage to the motors any faster than mMaxChange.
    //
    double mLastVoltage ;

    //
    // The maximum voltage change per second allowed on the motors
    //
    double mMaxChange ;

    //
    // If true continue to dump data while in the IDLE mode, used to detect drift
    //
    bool mDataDumpMode ;

    //
    // The start time for dumping data in IDLE mode, allows us to limit the IDLE data
    // dump to a five second time period
    //
    double mDataDumpStartTime ;

    //
    // The number of cycles to skip before processing the target distance or angle
    //
    int mCycleInterval ;

    //
    // The current cycle number for the drivebase
    //
    int mCurrentCycle ;

    //
    // The previous voltage for the motors
    //
    double mLastLeftVoltage ;
    double mLastRightVoltage ;

#ifdef TUNING
    static std::string AngleTargetName ;
    static std::string AngleActualName ;
    static std::string AngleVelocityName ;
    static std::string AnglePName ;
    static std::string AngleIName ;
    static std::string AngleDName ;
	static std::string AngleTimeName ;
#endif
};

#endif
