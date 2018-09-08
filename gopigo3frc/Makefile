CXX = arm-linux-gnueabihf-g++

CXXFLAGS=-I../gopigo3hw

SOURCES = \
	DriverStation.cpp\
	Encoder.cpp\
	GenericHID.cpp\
	GPG3MotorController.cpp\
	I2C.cpp\
	Joystick.cpp\
	RobotBase.cpp\
	SampleRobot.cpp\
	SerialPort.cpp\
	Servo.cpp\
	Timer.cpp

TARGET=gopigo3frc

OBJECTS = $(SOURCES:.cpp=.o)

$(TARGET).a : $(OBJECTS)
	$(AR) rvs $@ $(OBJECTS)
