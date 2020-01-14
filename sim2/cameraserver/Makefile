TOPDIR=../..

SOURCES = \
	./cpp/cameraserver/CameraServer.cpp\
	./cpp/cameraserver/CameraServerShared.cpp\
	./cpp/vision/VisionRunner.cpp

TARGET=cameraserver

NEED_WPIUTIL_2=true
NEED_NTCORE_2=true
NEED_CSCORE_2=true

LOCAL_CFLAGS += -Iinclude -Icpp

include $(TOPDIR)/makefiles/buildlib.mk
