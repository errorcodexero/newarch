TOPDIR=../..

include $(TOPDIR)/makefiles/defaults.mk

SOURCES = \
	./src/rev/CANEncoder.cpp\
	./src/rev/CANSparkMax.cpp\
	./src/rev/CANSparkMaxLowLevel.cpp\

TARGET=rev

NEED_WPIUTIL=true
NEED_NTCORE=true
NEED_WPILIB=true
NEED_ENGINE=true
NEED_HAL=true

LOCAL_CFLAGS += -Iinclude/rev

include $(TOPDIR)/makefiles/buildlib.mk
