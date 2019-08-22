TOPDIR=../..

AUTOMODES = \

SUBSYSTEMS = \
	src/main/cpp/swervesubsystem/SwerveDriveBase.cpp\
	src/main/cpp/swervesubsystem/SwerveTestAction.cpp

SOURCES = \
	src/main/cpp/main.cpp\
	src/main/cpp/SwerveRobot.cpp\
	$(SUBSYSTEMS)\
	$(AUTOMODES)

TARGET = swerve

NEED_WPILIB=true
NEED_WPIUTIL=true
NEED_CTRE=true
NEED_SPARKMAX=true
NEED_NAVX=true
NEED_NETWORKTABLES=true
NEED_XEROMISC=true
NEED_XEROBASE=true
NEED_MODELS=true

LOCAL_CFLAGS = -Isrc/main/cpp

ifeq ($(PLATFORM),SIMULATOR)
	LOCAL_CFLAGS += -I$(TOPDIR)/robotsims/phasersim
	LOCAL_LIBS = $(TOPDIR)/robotsims/phasersim
	LOCAL_EXTERNAL_LIBS=$(TOPDIR)/external/dinput/dinput8.lib $(TOPDIR)/external/dinput/dxguid.lib -lcurses
endif

SUPPORTED_PLATFORMS=ROBORIO SIMULATOR

ifeq ($(PLATFORM),GOPIGO)
$(error This robot is not supported by the GOPIGO platform)
endif

include $(TOPDIR)/makefiles/buildexe.mk
