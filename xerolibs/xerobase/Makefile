TOPDIR=../..


#
# Note, the lifter, single motor subsystem, and camera tracker are excluded as 
# they are not compatible with the GoPiGo3 robot.
#


ifeq ($(PLATFORM),SIMULATOR)
SIMSOURCES = \
	lifter/Lifter.cpp\
	lifter/LifterCalibrateAction.cpp\
	lifter/LifterGoToHeightAction.cpp\
	lifter/LifterPowerAction.cpp\
	singlemotorsubsystem/SingleMotorSubsystem.cpp\
	singlemotorsubsystem/SingleMotorPowerAction.cpp\
	cameratracker/CameraTracker.cpp\
	cameratracker/CameraChangeAction.cpp
endif

SOURCES = \
	SequenceAction.cpp\
	AutoMode.cpp\
	AutoController.cpp\
	ControllerBase.cpp\
	DelayAction.cpp\
	TerminateAction.cpp\
	DispatchAction.cpp\
	ParallelAction.cpp\
	NTPathDataWatcher.cpp\
	Robot.cpp\
	PlotManager.cpp\
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
	tankdrive/modes/FollowPathAutomode.cpp\
	tankdrive/modes/TankDriveScrubMode.cpp\
	tankdrive/modes/StraightCharAutoMode.cpp\
	tankdrive/actions/TankDriveDistanceAction.cpp\
	tankdrive/actions/TankDriveVelocityAction.cpp\
	tankdrive/actions/TankDriveCharAction.cpp\
	tankdrive/actions/TankDriveAngleAction.cpp\
	tankdrive/actions/TankDriveAngleCharAction.cpp\
	tankdrive/actions/TankDrivePowerAction.cpp\
	tankdrive/actions/TankDriveTimedPowerAction.cpp\
	tankdrive/actions/TankDriveFollowPathAction.cpp\
	tankdrive/actions/TankDriveScrubCharAction.cpp\
	$(SIMSOURCES)

TARGET=xerobase

NEED_WPIUTIL=true
NEED_WPILIB=true
NEED_NAVX=true
NEED_CTRE=true
NEED_NETWORKTABLES=true
NEED_XEROMISC=true

LOCAL_CFLAGS = -I. -Itankdrive -Itankdrive/modes -Itankdrive/actions


include $(TOPDIR)/makefiles/buildlib.mk

