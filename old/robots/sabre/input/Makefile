CXX = arm-frc-linux-gnueabi-g++
CXXFLAGS = -std=c++14

SRC = \
	joystick_linux.cpp\
	panel.cpp\
	range_finder.cpp\
	util.cpp

OBJS = $(SRC:.cpp=.o)


input.a: $(OBJS)
	$(AR) r $@ $(OBJS)
