#
# Setup the include paths based on the what stanard components are needed
# and the target platform
#

#
# These are for the WPIutils
#
ifeq ($(NEED_WPIUTIL),true)

#
# WPIUTIL for the GOPIGO3
#
ifeq ($(PLATFORM),GOPIGO)
endif

ifeq ($(PLATFORM),SIMULATOR)
CXXFLAGS += -I$(TOPDIR)/simulator/wpiutilsim/include
endif

endif


#
# These are for the WPIlib
#
ifeq ($(NEED_WPILIB),true)

#
# WPILIB for the GOPIGO3
#
ifeq ($(PLATFORM),GOPIGO)
CXXFLAGS += -I$(TOPDIR)/gopigo3/gopigo3frc
endif

ifeq ($(PLATFORM),SIMULATOR)
CXXFLAGS += -I$(TOPDIR)/simulator/wpilibsim
endif

endif

#
# There are for the NAVX
#
ifeq ($(NEED_NAVX),true)
#
# NAVX for the GOPIGO3
#
ifeq ($(PLATFORM),GOPIGO)
CXXFLAGS += -I$(TOPDIR)/gopigo3/gopigo3navx
endif

ifeq ($(PLATFORM),SIMULATOR)
CXXFLAGS += -I$(TOPDIR)/simulator/navxsim
endif

endif

#
# There are for the CTRE libraries
#
ifeq ($(NEED_NAVX),true)
#
# CTRE FOR THE GOPIGO2
#
ifeq ($(PLATFORM),GOPIGO)
#
# Break CTRE out of FRC for the gopigo3
#
endif

ifeq ($(PLATFORM),SIMULATOR)
CXXFLAGS += -I$(TOPDIR)/simulator/ctresim
endif

endif
