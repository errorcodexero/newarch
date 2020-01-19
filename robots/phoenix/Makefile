TOPDIR=../..

SUBSYSTEMS = \
	./src/main/cpp/phlifter/Lifter.cpp\
	./src/main/cpp/phlifter/LifterCalibrateAction.cpp\
	./src/main/cpp/collector/CollectCubeAction.cpp\
	./src/main/cpp/collector/Collector.cpp\
	./src/main/cpp/collector/CollectorEjectCubeAction.cpp\
	./src/main/cpp/grabber/Grabber.cpp\
	./src/main/cpp/grabber/GrabberCalibrateAction.cpp\
	./src/main/cpp/liftingcollector/LiftingCollector.cpp\
	./src/main/cpp/phoenixoi/PhoenixOIDevice.cpp\
	./src/main/cpp/phoenixoi/PhoenixOISubsystem.cpp\
	./src/main/cpp/phoenixsubsystem/PhoenixRobotSubsystem.cpp\
	./src/main/cpp/wings/Wings.cpp\
	./src/main/cpp/wings/WingsDeployAction.cpp

AUTOMODES = \
	./src/main/cpp/automodes/PhoenixAutoController.cpp\
	./src/main/cpp/automodes/PhoenixAutoMode.cpp\
	./src/main/cpp/automodes/ThreeCubeAutoMode.cpp\
	./src/main/cpp/automodes/PhoenixTestTuneMode.cpp

SOURCES = \
	./src/main/cpp/main.cpp\
	./src/main/cpp/Phoenix.cpp\
	$(SUBSYSTEMS)\
	$(AUTOMODES)

TARGET = phoenix

ifeq ($(PLATFORM),SIMULATOR)
NEED_WPILIB=true
NEED_WPIUTIL=true
NEED_CTRE=true
NEED_REV=true
NEED_NAVX=true
NEED_NETWORKTABLES=true
NEED_XEROMISC=true
NEED_XEROBASE=true
NEED_MODELS=true

endif

ifeq ($(PLATFORM),SIM2)
NEED_XEROMISC=true
NEED_XEROBASE=true
NEED_NAVX=true
NEED_CTRE=true
NEED_REV=true
NEED_WPILIB=true
NEED_CAMERASERVER=true
NEED_CSCORE=true
NEED_NTCORE=true
NEED_WPIUTIL=true
NEED_HAL=true
NEED_MODELS=true
NEED_ENGINE=true
endif

LOCAL_CFLAGS = -Isrc/main/cpp

SUPPORTED_PLATFORMS=ROBORIO SIMULATOR

ifeq ($(PLATFORM),SIMULATOR)
	LOCAL_CFLAGS += -I$(TOPDIR)/robotsims/phoenixsim
	LOCAL_LIBS = $(TOPDIR)/robotsims/phoenixsim
	LOCAL_EXTERNAL_LIBS=-lcurses
endif

ifeq ($(PLATFORM),SIM2)
#	LOCAL_CFLAGS += -I$(TOPDIR)/robotsims/phoenixsim2
#	LOCAL_LIBS = $(TOPDIR)/robotsims/phoenixsim2
endif

include $(TOPDIR)/makefiles/buildexe.mk

