TOPDIR=../..

include $(TOPDIR)/makefiles/defaults.mk

AUTOMODES = \
	src/main/cpp/automodes/DroidAutoModeController.cpp\
	src/main/cpp/automodes/DroidAutoMode.cpp\
	src/main/cpp/automodes/DroidTestTuneMode.cpp

SUBSYSTEMS = \
	src/main/cpp/droidsubsystem/DroidSubsystem.cpp\
	src/main/cpp/gamepiecemanipulator/shooter/Shooter.cpp\
	src/main/cpp/gamepiecemanipulator/intake/Intake.cpp\
	src/main/cpp/gamepiecemanipulator/conveyor/Conveyor.cpp\
	src/main/cpp/gamepiecemanipulator/GamePieceManipulator.cpp\
	src/main/cpp/turret/Turret.cpp\
	src/main/cpp/controlpanelrotator/ControlPanelRotator.cpp\
	src/main/cpp/climber/Climber.cpp\
    src/main/cpp/controlpanelrotator/RotateControlPanelAction.cpp

SOURCES = \
	src/main/cpp/main.cpp\
	src/main/cpp/Droid.cpp\
	src/main/cpp/droidoi/DroidOIDevice.cpp\
	src/main/cpp/droidoi/DroidOISubsystem.cpp\
	$(SUBSYSTEMS)\
	$(AUTOMODES)

TARGET = droid

ifeq ($(PLATFORM),SIMULATOR)
NEED_WPILIB=true
NEED_WPIUTIL=true
NEED_CTRE=true
NEED_REV=true
NEED_NAVX=true
NEED_NETWORKTABLES=true
NEED_XEROMISC=true
NEED_XEROBASE=true
NEED_MODELS=true
endif

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

LOCAL_CFLAGS = -Isrc/main/cpp

SUPPORTED_PLATFORMS=ROBORIO SIMULATOR

ifeq ($(PLATFORM),SIMULATOR)
	LOCAL_CFLAGS += -I$(TOPDIR)/robotsims/droidsim
	LOCAL_LIBS = $(TOPDIR)/robotsims/droidsim
	LOCAL_EXTERNAL_LIBS=-lcurses -lpthread
endif

ifeq ($(PLATFORM),GOPIGO)
$(error This robot is not supported by the GOPIGO platform)
endif

include $(TOPDIR)/makefiles/buildexe.mk
