TOPDIR=../..

AUTOMODES = \
	./src/main/cpp/automodes/SwerveAutoModeController.cpp\
	./src/main/cpp/automodes/TestSwerveDriveModuleAutomode.cpp

SUBSYSTEMS = \
	./src/main/cpp/swervedrive/SwerveDriveBase.cpp\
	./src/main/cpp/swervedrive/SwerveDriveModuleTestAction.cpp\
	./src/main/cpp/swervedrive/SwerveModule.cpp\
	./src/main/cpp/swerveoi/SwerveOIDevice.cpp\
	./src/main/cpp/swerveoi/SwerveOISubsystem.cpp\
	./src/main/cpp/swerverobotsubsystem/SwerveRobotSubsystem.cpp

SOURCES = \
	./src/main/cpp/main.cpp\
	./src/main/cpp/SwerveRobot.cpp\
	$(SUBSYSTEMS)\
	$(AUTOMODES)

TARGET = swerve

NEED_WPILIB=true
NEED_WPIUTIL=true
NEED_CTRE=true
NEED_SPARK=true
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
