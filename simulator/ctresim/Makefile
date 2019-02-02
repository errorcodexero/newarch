TOPDIR=../..

SOURCES = \
	ctre/phoenix/MotorControl/CAN/TalonSRX.cpp\
	ctre/phoenix/MotorControl/CAN/VictorSPX.cpp\
	ctre/phoenix/MotorControl/IMotorController.cpp\
	ctre/phoenix/MotorControl/SensorCollection.cpp

TARGET=ctresim

NEED_WPILIB=true
NEED_NAVX=true
NEED_MODELS=true

LOCAL_CFLAGS += -I. -I$(TOPDIR)/xerolibs/xeromisc

include $(TOPDIR)/makefiles/buildlib.mk

