#
# This sets up platform specific stuff.  This is mostly defines
#


ifeq ($(PLATFORM),GOPIGO)
CXXFLAGS += -DGOPIGO
PLATFORMLIBS=-lwiringPi -lpthread -latomic
endif

ifeq ($(PLATFORM),SIMULATOR)
CXXFLAGS += -DSIMULATOR
endif

ifeq ($(PLATFORM),SIM2)
CXXFLAGS += -DSIM2 -pthread
LINKPOSTFIX += -pthread -ldl
ifeq ($(MYOS),Darwin)
ifndef OPENCV_BASE
OPENCV_BASE := $(shell brew --prefix opencv)
endif
CFLAGS += -I$(OPENCV_BASE)/include/opencv4
CXXFLAGS += -I$(OPENCV_BASE)/include/opencv4
endif
endif

