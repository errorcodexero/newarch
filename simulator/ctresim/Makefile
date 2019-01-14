TOPDIR=../..

SOURCES = \
	ctre/phoenix/MotorControl/CAN/TalonSRX.cpp\
	ctre/phoenix/MotorControl/SensorCollection.cpp

TARGET=ctresim

NEED_WPILIB=true
NEED_NAVX=true

LOCAL_CFLAGS += -I. -I$(TOPDIR)/xerolibs/xeromisc

include $(TOPDIR)/makefiles/buildlib.mk

