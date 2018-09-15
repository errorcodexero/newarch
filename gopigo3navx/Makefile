CXX = arm-linux-gnueabihf-g++

CXXFLAGS=-I../gopigo3hw -I.

SOURCES = \
	AHRS.cpp\
	ContinuousAngleTracker.cpp\
	InertialDataIntegrator.cpp\
	OffsetTracker.cpp\
	SerialIO.cpp\
	SerialPort.cpp\
	TimeStamp.cpp

TARGET=gopigo3navx

OBJECTS = $(SOURCES:.cpp=.o)

$(TARGET).a : $(OBJECTS)
	$(AR) rvs $@ $(OBJECTS)
