TOPDIR=../..

SOURCES = \
	ActionSequence.cpp\
	AutoMode.cpp\
	AutoController.cpp\
	ControllerBase.cpp\
	DelayAction.cpp\
	TerminateAction.cpp\
	DispatchAction.cpp\
	ParallelAction.cpp\
	Robot.cpp\
	RobotSubsystem.cpp\
	Subsystem.cpp\
	TCS34725ColorSensor.cpp\
	TeleopController.cpp\
	DetectAutoSequence.cpp\
	oi/DriverGamepad.cpp\
	oi/DriverGamepadRumbleAction.cpp\
	oi/OIDevice.cpp\
	oi/OIOutputAction.cpp\
	oi/OISubsystem.cpp\
	MessageListener/MessageListener.cpp\
	tankdrive/TankDrive.cpp\
	tankdrive/TankDriveDistanceAction.cpp\
	tankdrive/TankDriveVelocityAction.cpp\
	tankdrive/TankDriveCharAction.cpp\
	tankdrive/TankDriveAngleAction.cpp\
	tankdrive/TankDriveAngleCharAction.cpp\
	tankdrive/TankDrivePowerAction.cpp\
	tankdrive/TankDriveTimedPowerAction.cpp\
	tankdrive/TankDriveFollowPathAction.cpp\
	tankdrive/TankDriveScrubCharAction.cpp\
	tankdrive/LineDetectAction.cpp\
	tankdrive/LineFollowAction.cpp\
	lifter/Lifter.cpp\
	lifter/LifterCalibrateAction.cpp\
	lifter/LifterGoToHeightAction.cpp\
	lifter/LifterPowerAction.cpp\
	singlemotorsubsystem/SingleMotorPowerAction.cpp\
	singlemotorsubsystem/SingleMotorSubsystem.cpp\
	lightsensor/LightSensorSubsystem.cpp\
	cameratracker/CameraTracker.cpp\
	cameratracker/CameraChangeAction.cpp

TARGET=xerobase

NEED_WPIUTIL=true
NEED_WPILIB=true
NEED_NAVX=true
NEED_CTRE=true
NEED_NETWORKTABLES=true
NEED_XEROMISC=true

LOCAL_CFLAGS = -I.


include $(TOPDIR)/makefiles/buildlib.mk

