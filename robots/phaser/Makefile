TOPDIR=../..

SOURCES = \
	src/main/cpp/linefollower/LineFollower.cpp\
	src/main/cpp/main.cpp\
	src/main/cpp/Phaser.cpp\
	src/main/cpp/phaseroi/PhaserOIDevice.cpp\
	src/main/cpp/phaseroi/PhaserOISubsystem.cpp\
	src/main/cpp/phaserrobotsubsystem/PhaserRobotSubsystem.cpp\
	src/main/cpp/automodes/PhaserAutoModeController.cpp

TARGET = phaser

NEED_WPILIB=true
NEED_WPIUTIL=true
NEED_CTRE=true
NEED_NAVX=true
NEED_NETWORKTABLES=true
NEED_XEROMISC=true
NEED_XEROBASE=true

LOCAL_CFLAGS = -Isrc/main/cpp

ifeq ($(PLATFORM),SIMULATOR)
	LOCAL_CFLAGS += -I$(TOPDIR)/robotsims/phasersim
	LOCAL_LIBS = $(TOPDIR)/robotsims/phasersim
	LOCAL_EXTERNAL_LIBS=$(TOPDIR)/external/dinput/dinput8.lib $(TOPDIR)/external/dinput/dxguid.lib -lcurses
endif

ifeq ($(PLATFORM),GOPIGO)
$(error This robot is not supported by the GOPIGO platform)
endif

include $(TOPDIR)/makefiles/buildexe.mk


