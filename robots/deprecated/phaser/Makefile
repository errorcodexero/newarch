TOPDIR=../..

AUTOMODES = \
	src/main/cpp/automodes/PhaserAutoModeController.cpp\
	src/main/cpp/automodes/PhaserAutoModeBase.cpp\
	src/main/cpp/automodes/LifterHeightMode.cpp\
	src/main/cpp/automodes/CenterHabTwoHatch.cpp\
	src/main/cpp/automodes/LeftRocketTwoHatch.cpp\
	src/main/cpp/automodes/RightRocketTwoHatch.cpp

SUBSYSTEMS = \
	src/main/cpp/phaserrobotsubsystem/PhaserRobotSubsystem.cpp\
	src/main/cpp/phaserrobotsubsystem/ExtendClimberAction.cpp\
	src/main/cpp/phaserrobotsubsystem/StrafeAction.cpp\
	src/main/cpp/turntable/Turntable.cpp\
	src/main/cpp/turntable/TurntableCalibrateAction.cpp\
	src/main/cpp/turntable/TurntableGoToAngleAction.cpp\
	src/main/cpp/turntable/TurntablePowerAction.cpp\
	src/main/cpp/phasercameratracker/PhaserCameraTracker.cpp\
	src/main/cpp/phasercameratracker/DriveByVisionAction.cpp\
	src/main/cpp/carloshatch/CarlosHatch.cpp\
	src/main/cpp/carloshatch/CarlosHatchArmAction.cpp\
	src/main/cpp/carloshatch/CarlosHatchImpactAction.cpp\
	src/main/cpp/carloshatch/DumpHatch.cpp\
	src/main/cpp/cargoholder/CargoHolder.cpp\
	src/main/cpp/cargointake/CargoIntake.cpp\
	src/main/cpp/cargointake/CargoIntakeAction.cpp\
	src/main/cpp/climber/Climber.cpp\
	src/main/cpp/climber/ClimberDeployAction.cpp\
	src/main/cpp/gamepiecemanipulator/GamePieceManipulator.cpp\
	src/main/cpp/gamepiecemanipulator/CompleteLSCargoCollect.cpp\
	src/main/cpp/gamepiecemanipulator/FloorCollectCargoAction.cpp\
	src/main/cpp/gamepiecemanipulator/ScoreCargo.cpp\
	src/main/cpp/gamepiecemanipulator/ReadyAction.cpp\
	src/main/cpp/gamepiecemanipulator/ResetIntakesAction.cpp\
	src/main/cpp/gamepiecemanipulator/CalibrateManip.cpp\
	src/main/cpp/lightsensor/LineFollowAction.cpp\
	src/main/cpp/lightsensor/LightSensorSubsystem.cpp
	
SOURCES = \
	src/main/cpp/main.cpp\
	src/main/cpp/Phaser.cpp\
	src/main/cpp/phaseroi/PhaserOIDevice.cpp\
	src/main/cpp/phaseroi/PhaserOISubsystem.cpp\
	$(SUBSYSTEMS)\
	$(AUTOMODES)

TARGET = phaser

NEED_WPILIB=true
NEED_WPIUTIL=true
NEED_CTRE=true
NEED_NAVX=true
NEED_NETWORKTABLES=true
NEED_XEROMISC=true
NEED_XEROBASE=true
NEED_MODELS=true
DEFAULT_CONFIG=Debug
DEFAULT_PLATFORM=SIMULATOR

LOCAL_CFLAGS = -Isrc/main/cpp

SUPPORTED_PLATFORMS=ROBORIO SIMULATOR

ifeq ($(CONFIG),)
$(info CONFIG is unspecified.  Setting it to default value (${DEFAULT_CONFIG}).)
CONFIG := $(DEFAULT_CONFIG)
endif

ifeq ($(PLATFORM),)
$(info PLATFORM is unspecified.  Setting it to default value (${DEFAULT_PLATFORM}).)
PLATFORM := $(DEFAULT_PLATFORM)
endif

ifeq ($(PLATFORM),SIMULATOR)
	LOCAL_CFLAGS += -I$(TOPDIR)/robotsims/phasersim
	LOCAL_LIBS = $(TOPDIR)/robotsims/phasersim
	LOCAL_EXTERNAL_LIBS=-lcurses
endif

ifeq ($(PLATFORM),GOPIGO)
$(error This robot is not supported by the GOPIGO platform)
endif

include $(TOPDIR)/makefiles/buildexe.mk
