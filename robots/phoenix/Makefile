TOPDIR=../..

SOURCES = \
	./src/main/cpp/automodes/CenterSwitchLeft.cpp\
	./src/main/cpp/automodes/CenterSwitchRight.cpp\
	./src/main/cpp/automodes/CrossTheLine.cpp\
	./src/main/cpp/automodes/PhoenixAutoController.cpp\
	./src/main/cpp/automodes/PhoenixAutoMode.cpp\
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
	./src/main/cpp/main.cpp\
	./src/main/cpp/phlifter/Lifter.cpp\
	./src/main/cpp/phlifter/LifterCalibrateAction.cpp\
	./src/main/cpp/phlifter/LifterGoToHeightAction.cpp\
	./src/main/cpp/phlifter/LifterHoldClimbAction.cpp\
	./src/main/cpp/phlifter/LifterLowGearAction.cpp\
	./src/main/cpp/phlifter/LifterPowerAction.cpp\
	./src/main/cpp/Phoenix.cpp\
	./src/main/cpp/phoenixoi/PhoenixOIDevice.cpp\
	./src/main/cpp/phoenixoi/PhoenixOISubsystem.cpp\
	./src/main/cpp/phoenixsubsystem/DriveAndCollectAction.cpp\
	./src/main/cpp/phoenixsubsystem/PhoenixRobotSubsystem.cpp\
	./src/main/cpp/wings/Wings.cpp\
	./src/main/cpp/wings/WingsDeployAction.cpp


TARGET = phoenix

NEED_WPILIB=true
NEED_WPIUTIL=true
NEED_CTRE=true
NEED_NAVX=true
NEED_NETWORKTABLES=true
NEED_XEROMISC=true
NEED_XEROBASE=true

LOCAL_CFLAGS = -Isrc/main/cpp

ifeq ($(PLATFORM),SIMULATOR)
	LOCAL_CFLAGS += -I$(TOPDIR)/robotsims/phoenixsim
	LOCAL_LIBS = $(TOPDIR)/robotsims/phoenixsim
	LOCAL_EXTERNAL_LIBS=$(TOPDIR)/external/dinput/dinput8.lib $(TOPDIR)/external/dinput/dxguid.lib -lcurses
endif

include $(TOPDIR)/makefiles/buildexe.mk

