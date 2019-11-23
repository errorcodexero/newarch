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
	cameratracker/LimeLight.cpp\
	cameratracker/CameraChangeAction.cpp
endif

SOURCES = \
	actions/SequenceAction.cpp\
	actions/DelayAction.cpp\
	actions/TerminateAction.cpp\
	actions/DispatchAction.cpp\
	actions/ParallelAction.cpp\
	AutoMode.cpp\
	AutoController.cpp\
	ControllerBase.cpp\
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
	tankdrive/TankDrive.cpp\
	tankdrive/modes/FollowPathAutomode.cpp\
	tankdrive/modes/ScrubCharMode.cpp\
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
	motors/CTREMotorController.cpp\
	motors/MotorFactory.cpp\
	motors/MotorGroupController.cpp\
	motors/SparkMaxMotorController.cpp\
	motorencodersubsystem/MotorEncoderSubsystem.cpp\
	motorencodersubsystem/MotorEncoderGoToAction.cpp\
    motorencodersubsystem/MotorEncoderHoldAction.cpp\
    motorencodersubsystem/XeroEncoder.cpp\
	$(SIMSOURCES)

TARGET=xerobase

NEED_WPIUTIL=true
NEED_WPILIB=true
NEED_NAVX=true
NEED_CTRE=true
NEED_REV=true
NEED_NETWORKTABLES=true
NEED_XEROMISC=true

LOCAL_CFLAGS = -I. -Itankdrive -Itankdrive/modes -Itankdrive/actions


include $(TOPDIR)/makefiles/buildlib.mk

