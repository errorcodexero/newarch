TOPDIR=../..

AUTOMODES = \
	src/main/cpp/automodes/droidAutoModeController.cpp\
	src/main/cpp/automodes/droidAutoModeBase.cpp

SUBSYSTEMS = \
	src/main/cpp/droidsubsystem/droidSubsystem.cpp\

SOURCES = \
	src/main/cpp/main.cpp\
	src/main/cpp/droid.cpp\
	src/main/cpp/droidoi/droidOIDevice.cpp\
	src/main/cpp/droidoi/droidOISubsystem.cpp\
	$(SUBSYSTEMS)\
	$(AUTOMODES)

TARGET = droid

NEED_WPILIB=true
NEED_WPIUTIL=true
NEED_CTRE=true
NEED_NAVX=true
NEED_NETWORKTABLES=true
NEED_XEROMISC=true
NEED_XEROBASE=true
NEED_MODELS=true
NEED_REV=true
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
	LOCAL_CFLAGS += -I$(TOPDIR)/robotsims/droidsim
	LOCAL_LIBS = $(TOPDIR)/robotsims/droidsim
	LOCAL_EXTERNAL_LIBS=-lcurses
endif

ifeq ($(PLATFORM),GOPIGO)
$(error This robot is not supported by the GOPIGO platform)
endif

include $(TOPDIR)/makefiles/buildexe.mk
