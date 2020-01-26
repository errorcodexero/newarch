TOPDIR=../..

include $(TOPDIR)/makefiles/defaults.mk

SOURCES = \
	./cpp/cameraserver/CameraServer.cpp\
	./cpp/cameraserver/CameraServerShared.cpp\
	./cpp/vision/VisionRunner.cpp

TARGET=cameraserver

NEED_WPIUTIL=true
NEED_NTCORE=true
NEED_CSCORE=true

LOCAL_CFLAGS += -Iinclude -Icpp

include $(TOPDIR)/makefiles/buildlib.mk
