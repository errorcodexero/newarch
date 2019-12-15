TOPDIR=../..

SUBSYSTEMS = \
	./src/main/cpp/collector/CollectCubeAction.cpp\
	./src/main/cpp/collector/Collector.cpp\
	./src/main/cpp/collector/CollectorEjectCubeAction.cpp\
	./src/main/cpp/grabber/Grabber.cpp\
	./src/main/cpp/grabber/GrabberCalibrateAction.cpp\
	./src/main/cpp/grabber/GrabberHoldCubeAction.cpp\
	./src/main/cpp/grabber/GrabberPowerAction.cpp\
	./src/main/cpp/grabber/GrabberToAngleAction.cpp\
	./src/main/cpp/intake/Intake.cpp\
	./src/main/cpp/intake/IntakeAction.cpp\
	./src/main/cpp/intake/IntakeDutyCycleAction.cpp\
	./src/main/cpp/liftingcollector/LiftingCollectCubeAction.cpp\
	./src/main/cpp/liftingcollector/LiftingCollector.cpp\
	./src/main/cpp/phoenixoi/PhoenixOIDevice.cpp\
	./src/main/cpp/phoenixoi/PhoenixOISubsystem.cpp\
	./src/main/cpp/phoenixsubsystem/DriveAndCollectAction.cpp\
	./src/main/cpp/phoenixsubsystem/PhoenixRobotSubsystem.cpp\
	./src/main/cpp/wings/Wings.cpp\
	./src/main/cpp/wings/WingsDeployAction.cpp

AUTOMODES = \
	./src/main/cpp/automodes/PhoenixAutoController.cpp

SOURCES = \
	./src/main/cpp/main.cpp\
	./src/main/cpp/phlifter/Lifter.cpp\
	./src/main/cpp/phlifter/LifterCalibrateAction.cpp\
	./src/main/cpp/phlifter/LifterGoToHeightAction.cpp\
	./src/main/cpp/phlifter/LifterHoldClimbAction.cpp\
	./src/main/cpp/phlifter/LifterLowGearAction.cpp\
	./src/main/cpp/phlifter/LifterPowerAction.cpp\
	./src/main/cpp/Phoenix.cpp\
	$(SUBSYSTEMS)\
	$(AUTOMODES)



TARGET = phoenix

NEED_WPILIB=true
NEED_WPIUTIL=true
NEED_CTRE=true
NEED_REV=true
NEED_NAVX=true
NEED_NETWORKTABLES=true
NEED_XEROMISC=true
NEED_XEROBASE=true
NEED_MODELS=true
DEFAULT_CONFIG=Debug
DEFAULT_PLATFORM=SIMULATOR

LOCAL_CFLAGS = -Isrc/main/cpp

SUPPORTED_PLATFORMS=ROBORIO SIMULATOR

ifeq ($(CONFIG),)
$(info CONFIG is unspecified.  Setting it to default value (${DEFAULT_CONFIG}).)
CONFIG := $(DEFAULT_CONFIG)
endif

ifeq ($(PLATFORM),)
$(info PLATFORM is unspecified.  Setting it to default value (${DEFAULT_PLATFORM}).)
PLATFORM := $(DEFAULT_PLATFORM)
endif

ifeq ($(PLATFORM),SIMULATOR)
	LOCAL_CFLAGS += -I$(TOPDIR)/robotsims/phoenixsim
	LOCAL_LIBS = $(TOPDIR)/robotsims/phoenixsim
	LOCAL_EXTERNAL_LIBS=-lcurses
endif

include $(TOPDIR)/makefiles/buildexe.mk

