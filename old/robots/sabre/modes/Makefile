CXX = arm-frc-linux-gnueabi-g++
CXXFLAGS = -std=c++14

SRC = \
	teleop.cpp


OBJS = $(SRC:.cpp=.o)


modes.a: $(OBJS)
	$(AR) r $@ $(OBJS)
