TOPDIR=../..

SOURCES = \
	rev/CANSparkMax.cpp\
	rev/CANSparkMaxLowLevel.cpp

TARGET=revsim

NEED_WPILIB=true
NEED_NAVX=true
NEED_MODELS=true
NEED_CTRE=true

LOCAL_CFLAGS += -I. -I$(TOPDIR)/xerolibs/xeromisc

include $(TOPDIR)/makefiles/buildlib.mk

