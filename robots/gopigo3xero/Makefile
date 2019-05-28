TOPDIR=../..

AUTOMODES = \
	src/automodes/GoPiGo3AutoModeController.cpp\
	src/automodes/LedAutoMode.cpp

SUBSYSTEMS = \
	src/servosubsystem/ServoGoToAngle.cpp\
	src/servosubsystem/ServoSubsystem.cpp\
	src/gopigo3sub/GoPiGo3Subsystem.cpp\
	src/oi/GoPiGo3OIDevice.cpp\
	src/oi/GoPiGo3OISubsystem.cpp\
	src/ledsubsystem/LEDSubsystem.cpp\
	src/ledsubsystem/LEDSubsystemOnOffAction.cpp\
	src/ledsubsystem/LEDSubsystemBlinkAction.cpp

SOURCES = \
	src/GoPiGo3Xero.cpp\
	$(SUBSYSTEMS)\
	$(AUTOMODES)


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
