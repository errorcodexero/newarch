TOPDIR=../..

include $(TOPDIR)/makefiles/defaults.mk

SOURCES = \
	./src/AHRS.cpp\
	./src/ContinuousAngleTracker.cpp\
	./src/InertialDataIntegrator.cpp\
	./src/OffsetTracker.cpp\
	./src/RegisterIO.cpp\
	./src/RegisterIOI2C.cpp\
	./src/RegisterIOMau.cpp\
	./src/RegisterIOSPI.cpp\
	./src/SerialIO.cpp

TARGET=navx

NEED_WPIUTIL=true
NEED_NTCORE=true
NEED_WPILIB=true
NEED_HAL=true

LOCAL_CFLAGS += -Iinclude -Icpp

include $(TOPDIR)/makefiles/buildlib.mk

