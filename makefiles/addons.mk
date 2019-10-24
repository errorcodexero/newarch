#
# Setup the include paths based on the what stanard components are needed
# and the target platform
#

#
# These are for the GoPIGo3 hardware
#
ifeq ($(NEED_GOPIGO3HW),true)

#
# WPIUTIL for the GOPIGO3
#
ifeq ($(PLATFORM),GOPIGO)
CXXFLAGS += -I$(TOPDIR)/gopigo3/hw
LIBLIST += $(TOPDIR)/gopigo3/hw
endif

ifeq ($(PLATFORM),SIMULATOR)
$(error the GoPiGo3 hardware is not supported in the simulation environment)
endif

endif

#
###########################################################################################
#

#
# These are for the WPIutils
#
ifeq ($(NEED_WPIUTIL),true)

#
# WPIUTIL for the GOPIGO3
#
ifeq ($(PLATFORM),GOPIGO)
CXXFLAGS += -I$(TOPDIR)/gopigo3/wpiutil/include/
LIBLIST += $(TOPDIR)/gopigo3/wpiutil
endif

ifeq ($(PLATFORM),SIMULATOR)
CXXFLAGS += -I$(TOPDIR)/simulator/wpiutilsim/include
LIBLIST += $(TOPDIR)/simulator/wpiutilsim
endif

endif

#
###########################################################################################
#

#
# These are for the WPIlib
#
ifeq ($(NEED_WPILIB),true)

#
# WPILIB for the GOPIGO3
#
ifeq ($(PLATFORM),GOPIGO)
CXXFLAGS += -I$(TOPDIR)/gopigo3/wpilib/include
LIBLIST += $(TOPDIR)/gopigo3/wpilib
endif

ifeq ($(PLATFORM),SIMULATOR)
CXXFLAGS += -I$(TOPDIR)/simulator/wpilibsim/include
CXXFLAGS += -I$(TOPDIR)/simulator/wpilibsim/cpp
LIBLIST += $(TOPDIR)/simulator/wpilibsim
endif

endif

#
###########################################################################################
#

#
# These are for the WPIlib
#
ifeq ($(NEED_MODELS),true)

#
# WPILIB for the GOPIGO3
#
ifeq ($(PLATFORM),GOPIGO)
$(error Simulation models are not supported by GOPIGO platform)
endif

ifeq ($(PLATFORM),SIMULATOR)
CXXFLAGS += -I$(TOPDIR)/simulator/models/include
CXXFLAGS += -I$(TOPDIR)/simulator/models/cpp
LIBLIST += $(TOPDIR)/simulator/models
endif

endif

#
###########################################################################################
#

#
# These are for the Network Tables
#
ifeq ($(NEED_NETWORKTABLES),true)

# GOPIGO3
ifeq ($(PLATFORM),GOPIGO)
CXXFLAGS += -I$(TOPDIR)/gopigo3/nt/include
LIBLIST += $(TOPDIR)/gopigo3/nt
endif

# CYGWIN SIMULATOR
ifeq ($(PLATFORM),SIMULATOR)
CXXFLAGS += -I$(TOPDIR)/simulator/ntsim/include
LIBLIST += $(TOPDIR)/simulator/ntsim
endif

endif

#
###########################################################################################
#

#
# There are for the NAVX
#
ifeq ($(NEED_NAVX),true)

# GOPIGO3
ifeq ($(PLATFORM),GOPIGO)
CXXFLAGS += -I$(TOPDIR)/gopigo3/navx/include
LIBLIST += $(TOPDIR)/gopigo3/navx
endif

# CYGWIN SIMULATOR
ifeq ($(PLATFORM),SIMULATOR)
CXXFLAGS += -I$(TOPDIR)/simulator/navxsim
LIBLIST += $(TOPDIR)/simulator/navxsim
endif

endif

#
###########################################################################################
#

#
# There are for the CTRE libraries
#
ifeq ($(NEED_CTRE),true)

# GOPIGO3
ifeq ($(PLATFORM),GOPIGO)
CXXFLAGS += -I$(TOPDIR)/gopigo3/ctre
LIBLIST += $(TOPDIR)/gopigo3/ctre
endif

# CYGWIN SIMULATOR
ifeq ($(PLATFORM),SIMULATOR)
CXXFLAGS += -I$(TOPDIR)/simulator/ctresim
LIBLIST += $(TOPDIR)/simulator/ctresim
endif

endif

#
###########################################################################################
#
#
# There are for the REV Spark Max libraries
#
ifeq ($(NEED_REV),true)

# GOPIGO3
ifeq ($(PLATFORM),GOPIGO)
CXXFLAGS += -I$(TOPDIR)/gopigo3/rev
LIBLIST += $(TOPDIR)/gopigo3/rev
endif

# CYGWIN SIMULATOR
ifeq ($(PLATFORM),SIMULATOR)
CXXFLAGS += -I$(TOPDIR)/simulator/revsim
LIBLIST += $(TOPDIR)/simulator/revsim
endif

endif

#
###########################################################################################
#

#
# These are for the XeroMisc
#
ifeq ($(NEED_XEROMISC),true)

#
# WPILIB for the GOPIGO3
#
ifeq ($(PLATFORM),GOPIGO)
CXXFLAGS += -I$(TOPDIR)/xerolibs/xeromisc
LIBLIST += $(TOPDIR)/xerolibs/xeromisc
endif

ifeq ($(PLATFORM),SIMULATOR)
CXXFLAGS += -I$(TOPDIR)/xerolibs/xeromisc
LIBLIST += $(TOPDIR)/xerolibs/xeromisc
endif

endif

#
###########################################################################################
#

#
# These are for the XeroBase
#
ifeq ($(NEED_XEROBASE),true)

#
# WPILIB for the GOPIGO3
#
ifeq ($(PLATFORM),GOPIGO)
CXXFLAGS += -I$(TOPDIR)/xerolibs/xerobase
LIBLIST += $(TOPDIR)/xerolibs/xerobase
endif

ifeq ($(PLATFORM),SIMULATOR)
CXXFLAGS += -I$(TOPDIR)/xerolibs/xerobase
LIBLIST += $(TOPDIR)/xerolibs/xerobase
endif

endif
