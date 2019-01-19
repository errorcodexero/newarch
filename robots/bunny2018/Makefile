TOPDIR=../..

SOURCES = \
	src/main/cpp/automodes/BunnyAutoMode.cpp\
	src/main/cpp/Bunny.cpp\
	src/main/cpp/bunnyoi/BunnyOIDevice.cpp\
	src/main/cpp/bunnyoi/BunnyOISubsystem.cpp\
	src/main/cpp/bunnysubsystem/BunnySubsystem.cpp\
	src/main/cpp/intake/Intake.cpp\
	src/main/cpp/main.cpp\
	src/main/cpp/shooter/Shooter.cpp\
	src/main/cpp/shooter/ShooterEjectAutoBallAction.cpp\
	src/main/cpp/shooter/ShooterEjectOneBallAction.cpp\
	src/main/cpp/shooter/ShooterStageBallAction.cpp


TARGET = bunny2018

NEED_WPILIB=true
NEED_WPIUTIL=true
NEED_CTRE=true
NEED_NAVX=true
NEED_NETWORKTABLES=true
NEED_XEROMISC=true
NEED_XEROBASE=true

LOCAL_CFLAGS = -Isrc/main/cpp

ifeq ($(PLATFORM),SIMULATOR)
	LOCAL_CFLAGS += -I$(TOPDIR)/robotsims/bunny2018sim
	LOCAL_LIBS = $(TOPDIR)/robotsims/bunny2018sim
	LOCAL_EXTERNAL_LIBS=$(TOPDIR)/external/dinput/dinput8.lib $(TOPDIR)/external/dinput/dxguid.lib -lcurses
endif

ifeq ($(PLATFORM),GOPIGO)
$(error This robot is not supported by the GOPIGO platform)
endif

include $(TOPDIR)/makefiles/buildexe.mk


