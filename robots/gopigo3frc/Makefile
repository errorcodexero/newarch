TOPDIR=../..

SOURCES = \
	src/myrobot.cpp


TARGET = gopigo3simple

NEED_GOPIGO3HW=true
NEED_WPILIB=true
NEED_WPIUTIL=true
NEED_NETWORKTABLES=true
NEED_XEROMISC=true
NEED_CTRE=true
DEFAULT_CONFIG=Debug
DEFAULT_PLATFORM=GOPIGO

LOCAL_CFLAGS = -Isrc

# Set defaults for CONFIG and PLATFORM if unspecified
ifeq ($(CONFIG),)
$(info CONFIG is unspecified.  Setting it to default value (${DEFAULT_CONFIG}).)
CONFIG := $(DEFAULT_CONFIG)
endif
ifeq ($(PLATFORM),)
$(info PLATFORM is unspecified.  Setting it to default value (${DEFAULT_PLATFORM}).)
PLATFORM := $(DEFAULT_PLATFORM)
endif

ifeq ($(GOPIGOIP),)
$(info GOPIGOIP is unspecified.  Setting it to the local machine (127.0.0.1))
GOPIGOIP=127.0.0.1
endif

ifeq ($(PLATFORM),SIMULATOR)
$(error this robot is not supported by the simulator)
endif

include $(TOPDIR)/makefiles/buildexe.mk
