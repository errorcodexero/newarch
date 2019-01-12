TOPDIR=..

SOURCES = \
	Kinematics.cpp\
	MessageDestFile.cpp\
	MessageDestSeqFile.cpp\
	MessageLogger.cpp\
	MessageLoggerData.cpp\
	PIDCtrl.cpp\
	SettingsParser.cpp\
	StallMonitor.cpp\
	TrapezoidalProfile.cpp

TARGET=xeromisc

NEED_WPILIB=true

ifeq ($(PLATFORM),GOPIGO)
LOCAL_CFLAGS = -I../gopigo3/gopigo3frc
endif

LOCAL_CFLAGS += -I../xeromath

include $(TOPDIR)/makefiles/buildlib.mk

