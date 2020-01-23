
ifeq ($(CONFIG),Debug)
else
ifeq ($(CONFIG),Release)
else
CONFIG=Debug
endif
endif

ifeq ($(PLATFORM),GOPIGO)
else
ifeq ($(PLATFORM),PI)
else
ifeq ($(PLATFORM),SIMULATOR)
else
ifeq ($(PLATFORM),SIM2)
else
PLATFORM=SIM2
endif
endif
endif
endif
