TOPDIR=../..


#
# Note, the lifter, single motor subsystem, and camera tracker are excluded as 
# they are not compatible with the GoPiGo3 robot.
#


ifeq ($(PLATFORM),SIMULATOR)
SIMSOURCES = \
	singlemotorsubsystem/SingleMotorSubsystem.cpp\
	singlemotorsubsystem/SingleMotorPowerAction.cpp\
	cameratracker/LimeLight.cpp
endif

SOURCES = \
	actions/Action.cpp\
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
	tankdrive/actions/TankDriveCharAction.cpp\
	tankdrive/actions/TankDriveAngleAction.cpp\
	tankdrive/actions/TankDrivePowerAction.cpp\
	tankdrive/actions/TankDriveTimedPowerAction.cpp\
	tankdrive/actions/TankDriveFollowPathAction.cpp\
	tankdrive/actions/TankDriveScrubCharAction.cpp\
	motors/CTREMotorController.cpp\
	motors/MotorFactory.cpp\
	motors/MotorGroupController.cpp\
	motors/SparkMaxMotorController.cpp\
	motors/VictorSPMotorController.cpp\
	motorencodersubsystem/MotorEncoderSubsystem.cpp\
	motorencodersubsystem/MotorEncoderGoToAction.cpp\
    motorencodersubsystem/MotorEncoderHoldAction.cpp\
	motorencodersubsystem/MotorEncoderPowerAction.cpp\
	motorencodersubsystem/MotorEncoderVelocityAction.cpp\
    motorencodersubsystem/XeroEncoder.cpp\
	$(SIMSOURCES)

TARGET=xerobase

ifeq ($(PLATFORM),SIMULATOR)
NEED_WPIUTIL=true
NEED_WPILIB=true
NEED_NAVX=true
NEED_CTRE=true
NEED_REV=true
NEED_NETWORKTABLES=true
NEED_XEROMISC=true
endif

ifeq ($(PLATFORM),SIM2)
NEED_WPIUTIL_2=true
NEED_WPILIB_2=true
NEED_CSCORE_2=true
NEED_NTCORE_2=true
NEED_XEROMISC=true
NEED_HAL_2=true
NEED_NAVX_2=true
NEED_CTRE_2=true
NEED_REV_2=true
endif

LOCAL_CFLAGS = -I. -Itankdrive -Itankdrive/modes -Itankdrive/actions -DUSE_NAVX


include $(TOPDIR)/makefiles/buildlib.mk

