TOPDIR=../..

include $(TOPDIR)/makefiles/defaults.mk

SOURCES = \
	./cpp/WestCoastDrive.cpp


TARGET=models
NEED_WPIUTIL=true
NEED_WPILIB=true
NEED_NAVX=true
NEED_CTRE=true
NEED_REV=true
NEED_HAL=true
NEED_ENGINE=true
NEED_XEROMISC=true

LOCAL_CFLAGS += -Iinclude

include $(TOPDIR)/makefiles/buildlib.mk


