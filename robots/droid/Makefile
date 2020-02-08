TOPDIR=../..

include $(TOPDIR)/makefiles/defaults.mk

AUTOMODES = \
	src/main/cpp/automodes/DroidAutoModeController.cpp\
	src/main/cpp/automodes/DroidAutoMode.cpp\
	src/main/cpp/automodes/DroidTestTuneMode.cpp\
	src/main/cpp/automodes/DroidEightBallAutomode.cpp\
	src/main/cpp/automodes/DroidFiveBallAutomode.cpp\
	src/main/cpp/automodes/DroidThreeBallAutomode.cpp

SUBSYSTEMS = \
	src/main/cpp/droidoi/DroidOIDevice.cpp\
	src/main/cpp/droidoi/DroidOISubsystem.cpp\
	src/main/cpp/droidsubsystem/DroidSubsystem.cpp\
	src/main/cpp/gamepiecemanipulator/conveyor/Conveyor.cpp\
	src/main/cpp/gamepiecemanipulator/conveyor/ConveyorStateAction.cpp\
	src/main/cpp/gamepiecemanipulator/conveyor/ConveyorEmitAction.cpp\
	src/main/cpp/gamepiecemanipulator/conveyor/ConveyorPrepareToEmitAction.cpp\
	src/main/cpp/gamepiecemanipulator/conveyor/ConveyorReceiveAction.cpp\
	src/main/cpp/gamepiecemanipulator/conveyor/ConveyorPrepareToReceiveAction.cpp\
	src/main/cpp/gamepiecemanipulator/FireAction.cpp\
	src/main/cpp/gamepiecemanipulator/shooter/ShooterVelocityAction.cpp\
	src/main/cpp/gamepiecemanipulator/shooter/Shooter.cpp\
	src/main/cpp/gamepiecemanipulator/intake/Intake.cpp\
        src/main/cpp/gamepiecemanipulator/intake/CollectOnAction.cpp\
        src/main/cpp/gamepiecemanipulator/intake/CollectOffAction.cpp\
	src/main/cpp/gamepiecemanipulator/GamePieceManipulator.cpp\
	src/main/cpp/gamepiecemanipulator/StartCollectAction.cpp\
	src/main/cpp/gamepiecemanipulator/StopCollectAction.cpp\
	src/main/cpp/gamepiecemanipulator/ShootTestingAction.cpp\
	src/main/cpp/turret/FollowTargetAction.cpp\
	src/main/cpp/turret/Turret.cpp\
	src/main/cpp/climber/Climber.cpp\
	src/main/cpp/controlpanelrotator/ControlPanelRotator.cpp\
        src/main/cpp/controlpanelrotator/RotateControlPanelAction.cpp\
        src/main/cpp/limelight/DroidLimeLight.cpp\
        src/main/cpp/targettracker/TargetTracker.cpp\

MODELS = \
	src/models/cpp/DroidOIModel.cpp\
	src/models/cpp/IntakeModel.cpp\
	src/models/cpp/ConveyorModel.cpp

SOURCES = \
	src/main/cpp/main.cpp\
	src/main/cpp/Droid.cpp\
	$(SUBSYSTEMS)\
	$(AUTOMODES)\
	$(MODELS)

TARGET = droid

ifeq ($(PLATFORM),SIM2)
NEED_XEROMISC=true
NEED_XEROBASE=true
NEED_NAVX=true
NEED_CTRE=true
NEED_REV=true
NEED_WPILIB=true
NEED_CAMERASERVER=true
NEED_CSCORE=true
NEED_NTCORE=true
NEED_WPIUTIL=true
NEED_HAL=true
NEED_MODELS=true
NEED_ENGINE=true
endif

LOCAL_CFLAGS = -Isrc/main/cpp -Isrc/models/cpp -Isrc/models/include

SUPPORTED_PLATFORMS=ROBORIO SIMULATOR

ifeq ($(PLATFORM),SIMULATOR)
	LOCAL_CFLAGS += -I$(TOPDIR)/robotsims/droidsim
	LOCAL_LIBS = $(TOPDIR)/robotsims/droidsim
	LOCAL_EXTERNAL_LIBS = -lcurses -lpthread
endif

ifeq ($(PLATFORM),GOPIGO)
$(error This robot is not supported by the GOPIGO platform)
endif

include $(TOPDIR)/makefiles/buildexe.mk

