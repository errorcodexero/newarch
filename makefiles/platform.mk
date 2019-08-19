#
# This sets up platform specific stuff.  This is mostly defines
#


ifeq ($(PLATFORM),GOPIGO)
CXXFLAGS += -DGOPIGO
PLATFORMLIBS=-lwiringPI -lpthread -latomic
endif

ifeq ($(PLATFORM),SIMULATOR)
CXXFLAGS += -DSIMULATOR
endif
