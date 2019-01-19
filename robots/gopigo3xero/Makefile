TOPDIR=../..

SOURCES = \
	src/GoPiGo3Xero.cpp\
	src/ServoGoToAngle.cpp\
	src/ServoSubsystem.cpp\
	src/GoPiGo3Subsystem.cpp\
	src/GoPiGo3AutoModeController.cpp\
	src/GoPiGo3OISubsystem.cpp\
	src/GoPiGo3OIDevice.cpp\
	src/LEDSubsystem.cpp\
	src/LEDSubsystemOnOffAction.cpp\
	src/LEDSubsystemBlinkAction.cpp


TARGET = gopigo3xero

NEED_GOPIGO3HW=true
NEED_WPILIB=true
NEED_WPIUTIL=true
NEED_CTRE=true
NEED_NAVX=true
NEED_NETWORKTABLES=true
NEED_XEROMISC=true
NEED_XEROBASE=true

LOCAL_CFLAGS = -Isrc

ifeq ($(PLATFORM),SIMULATOR)
$(error this robot is not supported by the simulator)
endif

include $(TOPDIR)/makefiles/buildexe.mk
