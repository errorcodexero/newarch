TOPDIR=../..

AUTOMODES = \
	src/main/cpp/automodes/RanseurAutoModeBase.cpp\
	src/main/cpp/automodes/RanseurAutoModeController.cpp

SUBSYSTEMS = \
	src/main/cpp/bunnyarm/BunnyArm.cpp\
	src/main/cpp/bunnyarm/BunnyArmDeployAction.cpp\
	src/main/cpp/ranseurcameratracker/DriveByVisionAction.cpp\
	src/main/cpp/ranseurcameratracker/RanseurCameraTracker.cpp\
	src/main/cpp/ranseuroi/RanseurOIDevice.cpp\
	src/main/cpp/ranseuroi/RanseurOISubsystem.cpp\
	src/main/cpp/ranseurrobotsubsystem/RanseurRobotSubsystem.cpp\
	src/main/cpp/tubmanipulator/arm/TubArm.cpp\
	src/main/cpp/tubcollector/Collector.cpp\
	src/main/cpp/tubcollector/CollectorDutyCycleAction.cpp\
	src/main/cpp/tubcollector/CollectorEjectTubAction.cpp\
	src/main/cpp/tubcollector/CollectorTubAction.cpp\
    src/main/cpp/tubmanipulatorsubsystem/TubManipulatorSubsystem.cpp\
    src/main/cpp/tubarm/TubArm.cpp\
    src/main/cpp/tubwrist/TubWrist.cpp\
    src/main/cpp/tubcollector/Tub
	
SOURCES = \
	src/main/cpp/main.cpp\
	src/main/cpp/Ranseur.cpp\
	$(SUBSYSTEMS)\
	$(AUTOMODES)

TARGET = ranseur

NEED_WPILIB=true
NEED_WPIUTIL=true
NEED_CTRE=true
NEED_REV=true
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
	LOCAL_CFLAGS += -I$(TOPDIR)/robotsims/ranseursim
	LOCAL_LIBS = $(TOPDIR)/robotsims/ranseursim
	LOCAL_EXTERNAL_LIBS=-lcurses -lpthread
endif

ifeq ($(PLATFORM),GOPIGO)
$(error This robot is not supported by the GOPIGO platform)
endif

include $(TOPDIR)/makefiles/buildexe.mk
