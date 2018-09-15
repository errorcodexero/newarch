
CXX = arm-linux-gnueabihf-g++
CXXFLAGS=-g $(LOCAL_CFLAGS) -Wno-psabi -DGOPIGO
OBJECTS = $(SOURCES:.cpp=.o)

$(TARGET).a : $(OBJECTS)
	$(AR) rvs $@ $(OBJECTS)

clean:
	rm -rf $(OBJECTS) $(TARGET).a

$(TARGET) : $(PILIBS) $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS) $(PILIBS) -lwiringPI -lpthread
