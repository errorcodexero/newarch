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

LOCAL_CFLAGS = -Isrc

ifeq ($(PLATFORM),SIMULATOR)
$(error this robot is not supported by the simulator)
endif

include $(TOPDIR)/makefiles/buildexe.mk
