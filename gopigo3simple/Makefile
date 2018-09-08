CXX = arm-linux-gnueabihf-g++

CXXFLAGS = -I../gopigo3frc -I../gopigo3hw -g

SOURCES = \
	myrobot.cpp

PILIBS = \
	../gopigo3frc/gopigo3frc.a\
	../gopigo3hw/gopigo3hw.a

TARGET=robot

OBJECTS = $(SOURCES:.cpp=.o)

$(TARGET) : $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS) $(PILIBS) -lwiringPI -lpthread

