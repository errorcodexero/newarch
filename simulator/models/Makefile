TOPDIR=../..

SOURCES = \
	./cpp/TankDriveModel.cpp\
	./cpp/LifterModel.cpp\
	./cpp/TurnTableModel.cpp

TARGET = models

NEED_WPIUTIL=true
NEED_WPILIB = true
NEED_NETWORKTABLES = true
NEED_NAVX = true
NEED_CTRE = true
NEED_XEROMISC=true

LOCAL_CFLAGS += \
	-Iinclude

include $(TOPDIR)/makefiles/buildlib.mk
