CXX = arm-frc-linux-gnueabi-g++
CXXFLAGS = -std=c++14 -I$(WPILIB)/cpp/current/include
LDFLAGS = -L$(WPILIB)/cpp/current/lib

MYLIBS = \
	../modes/modes.a\
	../control/control.a\
	../input/input.a\
	../util/util.a

SRC = \
	dio_control.cpp\
	jag_control.cpp\
	talon_srx_control.cpp\
	to_roborio.cpp

OBJS = $(SRC:.cpp=.o)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS) $(MYLIBS) $(LDFLAGS) -lwpi
