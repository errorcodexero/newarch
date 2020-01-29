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
endif

