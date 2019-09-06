TOPDIR=../..

SOURCES = \
	cpp/DigitalInput.cpp\
	cpp/DriverStation.cpp\
	cpp/Encoder.cpp\
	cpp/livewindow/LiveWindow.cpp\
	cpp/livewindow/LiveWindowSendable.cpp\
	cpp/PrintVisualizer.cpp\
	cpp/RobotSimBase.cpp\
	cpp/SimEventManager.cpp\
	cpp/SampleRobot.cpp\
	cpp/ScreenVisualizer.cpp\
	cpp/SimulatedObject.cpp\
	cpp/smartdashboard/NamedSendable.cpp\
	cpp/smartdashboard/SendableBase.cpp\
	cpp/smartdashboard/SendableBuilderImpl.cpp\
	cpp/smartdashboard/SendableChooserBase.cpp\
	cpp/smartdashboard/SmartDashboard.cpp\
	cpp/Solenoid.cpp\
	cpp/SubsystemModel.cpp\
	cpp/Timer.cpp\
	cpp/VictorSP.cpp\
	cpp/Relay.cpp

TARGET = wpilibsim

NEED_XEROBASE=true
NEED_WPIUTIL=true
NEED_NETWORKTABLES = true
NEED_NAVX = true
NEED_CTRE = true
NEED_XEROMISC=true
NEED_MODELS=true

LOCAL_CFLAGS += \
	-Iinclude\
	-Icpp\

include $(TOPDIR)/makefiles/buildlib.mk


