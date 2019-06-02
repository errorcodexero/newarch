TOPDIR=../..

SOURCES = \
	src/AHRS.cpp\
	src/ContinuousAngleTracker.cpp\
	src/InertialDataIntegrator.cpp\
	src/OffsetTracker.cpp\
	src/RegisterIO.cpp\
	src/RegisterIOI2C.cpp\
	src/RegisterIOSPI.cpp\
	src/SerialIO.cpp


TARGET=navx

LOCAL_CFLAGS=-Iinclude

NEED_WPILIB=true
NEED_WPIUTIL=true
NEED_NETWORKTABLES=true

include $(TOPDIR)/makefiles/buildlib.mk


