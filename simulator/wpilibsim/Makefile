TOPDIR=../..

SOURCES = \
	./cpp/DigitalInput.cpp\
	./cpp/DIJoystickManager.cpp\
	./cpp/DriverStation.cpp\
	./cpp/Encoder.cpp\
	./cpp/livewindow/LiveWindow.cpp\
	./cpp/livewindow/LiveWindowSendable.cpp\
	./cpp/PrintVisualizer.cpp\
	./cpp/RobotSimBase.cpp\
	./cpp/SampleRobot.cpp\
	./cpp/ScreenVisualizer.cpp\
	./cpp/SimulatedObject.cpp\
	./cpp/smartdashboard/NamedSendable.cpp\
	./cpp/smartdashboard/SendableBase.cpp\
	./cpp/smartdashboard/SendableBuilderImpl.cpp\
	./cpp/smartdashboard/SendableChooserBase.cpp\
	./cpp/smartdashboard/SmartDashboard.cpp\
	./cpp/Solenoid.cpp\
	./cpp/SubsystemModel.cpp\
	./cpp/Timer.cpp\
	./cpp/VictorSP.cpp\
	./cpp/TankDriveModel.cpp\
	./cpp/LifterModel.cpp\
	./cpp/TurnTableModel.cpp

TARGET = wpilibsim

LOCAL_CFLAGS += \
	-Iinclude\
	-Icpp\
	-I$(TOPDIR)/xerolibs/xeromath\
	-I$(TOPDIR)/xerolibs/xeromisc\
	-I$(TOPDIR)/simulator/wpiutilsim/include\
	-I$(TOPDIR)/simulator/ctresim\
	-I$(TOPDIR)/simulator/navxsim\
	-I$(TOPDIR)/simulator/ntsim/include

include $(TOPDIR)/makefiles/buildlib.mk


