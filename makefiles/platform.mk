#
# This sets up platform specific stuff.  This is mostly defines
#


ifeq ($(PLATFORM),GOPIGO)
CXXFLAGS += -DGOPIGO
PLATFORMLIBS=-lwiringPI -lpthread
endif

ifeq ($(PLATFORM),SIMULATOR)
CXXFLAGS += -DSIMULATOR
endif
