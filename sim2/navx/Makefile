TOPDIR=../..

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

NEED_WPIUTIL_2=true
NEED_NTCORE_2=true
NEED_WPILIB_2=true
NEED_HAL_2=true

LOCAL_CFLAGS += -Iinclude -Icpp

include $(TOPDIR)/makefiles/buildlib.mk

