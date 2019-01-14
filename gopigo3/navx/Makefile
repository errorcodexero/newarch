TOPDIR=../..

SOURCES = \
	AHRS.cpp\
	ContinuousAngleTracker.cpp\
	InertialDataIntegrator.cpp\
	OffsetTracker.cpp\
	RegisterIO.cpp\
	RegisterIOI2C.cpp\
	RegisterIOSPI.cpp\
	SerialIO.cpp

TARGET=navx

LOCAL_CFLAGS=-I.

NEED_WPILIB=true
NEED_WPIUTIL=true
NEED_NETWORKTABLES=true

include $(TOPDIR)/makefiles/buildlib.mk


