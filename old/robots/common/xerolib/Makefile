#
# makefile for the control library
#

#
# define our target library
#
TARGET=xerolib.a

#
# Define the external software we need
#
WPILIB=true

NAVX=true

#
# define the source code files
#
SRC = \
	AdaptivePurePursuitController.cpp\
	AutoDelay.cpp\
	AutonomousControllerBase.cpp\
	AutonomousStepBase.cpp\
	AutoSetMotorVoltage.cpp\
	DriveBase.cpp\
	MessageLogger.cpp\
	MessageStreamDestination.cpp\
	PIDCtrl.cpp\
	RobotControllerBase.cpp\
	SubsystemBase.cpp\
	XeroRobotBase.cpp\
	HeadingProfileFollower.cpp\
	MotionProfile.cpp\
	ParamsParser.cpp\
	Path.cpp\
	PathFollower.cpp\
	PathSegment.cpp\
	Position.cpp\
	ProfileFollower.cpp\
	ProfileGenerator.cpp\
	ProfileGoal.cpp\
	RobotState.cpp\
	RobotStateEstimator.cpp\
	Rotation.cpp\
	Segment.cpp\
	SetpointGenerator.cpp\
	State.cpp


#
# include a central make file that defines our compilation recipe
#
include ../../../makefiles/compilelib.mk

