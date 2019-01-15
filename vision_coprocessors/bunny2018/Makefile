SOURCES = \
	params_parser.cpp \
	main.cpp

TARGET = CrateDetector.exe

LIBS = \
	`pkg-config --libs opencv libv4l2`

CXXINC = \
	`pkg-config --cflags opencv` \
	-I../xeromisc

CXXFLAGS = -g $(CXXINC) -std=c++17 -DDEBUG

CXX = /usr/bin/g++
OBJECTS = $(SOURCES:.cpp=.o)

$(TARGET) : $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS) $(LIBS)

clean:
	rm -f $(TARGET) $(OBJECTS)
