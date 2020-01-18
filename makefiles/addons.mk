#
# Setup the include paths based on the what stanard components are needed
# and the target platform
#


###########################################################################################
#                                                                                         #
#                                      GoPiGo3                                            #
#                                                                                         #
###########################################################################################

#
# These are for the GoPIGo3 hardware
#
ifeq ($(NEED_GOPIGO3HW),true)

#
# GoPiGo3 hardware for the GOPIGO3
#
ifeq ($(PLATFORM),GOPIGO)
CXXFLAGS += -I$(TOPDIR)/gopigo3/hw
LIBLIST += $(TOPDIR)/gopigo3/hw
endif

ifeq ($(NEED_XEROBASE),true)
CXXFLAGS += -I$(TOPDIR)/xerolibs/xerobase
LIBLIST += $(TOPDIR)/xerolibs/xerobase
endif

ifeq ($(NEED_REV),true)
CXXFLAGS += -I$(TOPDIR)/gopigo3/rev
LIBLIST += $(TOPDIR)/gopigo3/rev
endif

ifeq ($(NEED_XEROMISC),true)
CXXFLAGS += -I$(TOPDIR)/xerolibs/xeromisc
LIBLIST += $(TOPDIR)/xerolibs/xeromisc
endif

ifeq ($(NEED_NETWORKTABLES),true)
CXXFLAGS += -I$(TOPDIR)/gopigo3/nt/include
LIBLIST += $(TOPDIR)/gopigo3/nt
endif

ifeq ($(NEED_WPIUTIL),true)
CXXFLAGS += -I$(TOPDIR)/gopigo3/wpiutil/include/
LIBLIST += $(TOPDIR)/gopigo3/wpiutil
endif

ifeq ($(NEED_WPILIB),true)
CXXFLAGS += -I$(TOPDIR)/gopigo3/wpilib/include
LIBLIST += $(TOPDIR)/gopigo3/wpilib
endif

ifeq ($(NEED_NAVX),true)
CXXFLAGS += -I$(TOPDIR)/gopigo3/navx/include
LIBLIST += $(TOPDIR)/gopigo3/navx
endif

ifeq ($(NEED_CTRE),true)
CXXFLAGS += -I$(TOPDIR)/gopigo3/ctre
LIBLIST += $(TOPDIR)/gopigo3/ctre
endif

endif

###########################################################################################
#                                                                                         #
#                                      Simulator                                          #
#                                                                                         #
###########################################################################################


ifeq ($(PLATFORM),SIMULATOR)

ifeq ($(NEED_XEROBASE),true)
CXXFLAGS += -I$(TOPDIR)/xerolibs/xerobase
LIBLIST += $(TOPDIR)/xerolibs/xerobase
endif

ifeq ($(NEED_XEROMISC),true)
CXXFLAGS += -I$(TOPDIR)/xerolibs/xeromisc
LIBLIST += $(TOPDIR)/xerolibs/xeromisc
endif

ifeq ($(NEED_NETWORKTABLES),true)
CXXFLAGS += -I$(TOPDIR)/simulator/ntsim/include
LIBLIST += $(TOPDIR)/simulator/ntsim
endif

ifeq ($(NEED_WPIUTIL),true)
CXXFLAGS += -I$(TOPDIR)/simulator/wpiutilsim/include
LIBLIST += $(TOPDIR)/simulator/wpiutilsim
endif

ifeq ($(NEED_WPILIB),true)
CXXFLAGS += -I$(TOPDIR)/simulator/wpilibsim/include -I$(TOPDIR)/simulator/wpilibsim/cpp
LIBLIST += $(TOPDIR)/simulator/wpilibsim
endif

ifeq ($(NEED_MODELS),true)
CXXFLAGS += -I$(TOPDIR)/simulator/models/include -I$(TOPDIR)/simulator/models/cpp
LIBLIST += $(TOPDIR)/simulator/models
endif

ifeq ($(NEED_NAVX),true)
CXXFLAGS += -I$(TOPDIR)/simulator/navxsim
LIBLIST += $(TOPDIR)/simulator/navxsim
endif

ifeq ($(NEED_CTRE),true)
CXXFLAGS += -I$(TOPDIR)/simulator/ctresim
LIBLIST += $(TOPDIR)/simulator/ctresim
endif

ifeq ($(NEED_REV),true)
CXXFLAGS += -I$(TOPDIR)/simulator/revsim
LIBLIST += $(TOPDIR)/simulator/revsim
endif

endif

###########################################################################################
#                                                                                         #
#                                      SIM2                                               #
#                                                                                         #
###########################################################################################

ifeq ($(PLATFORM),SIM2)

ifeq ($(NEED_XEROBASE),true)
CXXFLAGS += -I$(TOPDIR)/xerolibs/xerobase
LIBLIST += $(TOPDIR)/xerolibs/xerobase
endif

ifeq ($(NEED_XEROMISC),true)
CXXFLAGS += -I$(TOPDIR)/xerolibs/xeromisc
LIBLIST += $(TOPDIR)/xerolibs/xeromisc
endif

ifeq ($(NEED_NAVX),true)
CXXFLAGS += -I$(TOPDIR)/sim2/navx/include
LIBLIST += $(TOPDIR)/sim2/navx
endif

ifeq ($(NEED_CTRE),true)
CXXFLAGS += -I$(TOPDIR)/sim2/ctre/include
LIBLIST += $(TOPDIR)/sim2/ctre
endif

ifeq ($(NEED_REV),true)
CXXFLAGS += -I$(TOPDIR)/sim2/rev/include
LIBLIST += $(TOPDIR)/sim2/rev
endif

ifeq ($(NEED_WPILIB),true)
CXXFLAGS += -I$(TOPDIR)/sim2/wpilib/include
LIBLIST += $(TOPDIR)/sim2/wpilib
endif

ifeq ($(NEED_CAMERASERVER),true)
CXXFLAGS += -I$(TOPDIR)/sim2/cameraserver/include
LIBLIST += $(TOPDIR)/sim2/cameraserver
endif

ifeq ($(NEED_NTCORE),true)
CXXFLAGS += -I$(TOPDIR)/sim2/ntcore/include
LIBLIST += $(TOPDIR)/sim2/ntcore
endif

ifeq ($(NEED_CSCORE),true)
CXXFLAGS += -I$(TOPDIR)/sim2/cscore/include
LIBLIST += $(TOPDIR)/sim2/cscore
endif

ifeq ($(NEED_WPIUTIL),true)
CXXFLAGS += -I$(TOPDIR)/sim2/wpiutil/include -I$(TOPDIR)/sim2/wpiutil/eigeninclude -I$(TOPDIR)/sim2/wpiutil/libuv/include
LIBLIST += $(TOPDIR)/sim2/wpiutil
endif

ifeq ($(NEED_HAL),true)
CXXFLAGS += -I$(TOPDIR)/sim2/hal/include -I$(TOPDIR)/sim2/hal/build -I$(TOPDIR)/sim2/hal/sim2
LIBLIST += $(TOPDIR)/sim2/hal
endif

ifeq ($(NEED_MODELS),true)
CXXFLAGS += -I$(TOPDIR)/sim2/models/include -I$(TOPDIR)/sim2/models/cpp
LIBLIST += $(TOPDIR)/sim2/models
endif

endif
