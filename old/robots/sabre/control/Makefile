CXX = arm-frc-linux-gnueabi-g++
CXXFLAGS = -std=c++14

SRC = \
	climb_release.cpp\
	collector.cpp\
	drivebase.cpp\
	force.cpp\
	force_interface.cpp\
	front.cpp\
	log.cpp\
	main.cpp\
	monitor.cpp\
	motor_check.cpp\
	pump.cpp\
	shooter.cpp\
	sides.cpp\
	tilt.cpp\
	toplevel.cpp\
	tote_sensors.cpp\
	winch.cpp

OBJS = $(SRC:.cpp=.o)


control.a: $(OBJS)
	$(AR) r $@ $(OBJS)
