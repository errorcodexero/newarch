CXX = arm-linux-gnueabihf-g++

SOURCES = \
	GoPiGo3Robot.cpp\
	GoPiGo3SPI.cpp

TARGET=gopigo3hw

OBJECTS = $(SOURCES:.cpp=.o)

$(TARGET).a : $(OBJECTS)
	$(AR) rvs $@ $(OBJECTS)
