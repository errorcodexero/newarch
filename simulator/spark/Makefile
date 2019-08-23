TOPDIR=../..

SOURCES = \
	rev/CANSparkMaxLowLevel.cpp\
	rev/CANSparkMax.cpp


TARGET=spark

NEED_WPILIB=true
NEED_NAVX=true
NEED_MODELS=true

LOCAL_CFLAGS += -I. -I$(TOPDIR)/xerolibs/xeromisc

include $(TOPDIR)/makefiles/buildlib.mk
