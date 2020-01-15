#
# Setup the compilers
#

COMPILERSETUP=

ifeq ($(MYCOMPILER),PI)
CXX = arm-linux-gnueabihf-g++
AR = arm-linux-gnueabihf-ar
CXXFLAGS=$(LOCAL_CFLAGS) -Wno-psabi
COMPILERSETUP=true
LINKPREFIX=-Wl,--start-group
LINKPOSTFIX=-Wl,--end-group
EXEEXT=

ifdef MAKEDEBUG
$(info Setting compiler for PI)
endif

endif

ifeq ($(MYCOMPILER),GCC)
CXX = g++
CXXFLAGS = $(LOCAL_CFLAGS) -Wall
COMPILERSETUP=true

ifeq ($(MYOS),Windows)
EXEEXT=.exe
CXXFLAGS += -DCYGWIN
LINKPREFIX=-Wl,--start-group
LINKPOSTFIX=-Wl,--end-group
else
CXXFLAGS += -std=c++17
EXEECT=
LINKPREFIX=
LINKPOSTFIX=
endif

ifdef MAKEDEBUG
$(info Setting compiler for base GCC configuration)
endif

endif

ifeq ($(CONFIG),Debug)
CXXFLAGS += -g

#
# This is to work around something STUPID that first did in the network
# tables code.  They use the CPP macro DEBUG for something other than what
# the rest of the world uses it for.  Therefore, a client makefile can set
# the SUPRESS_DEBUG_DEFINE make varaible and we do not set the DEFINE preprocessor
# macro in the DEBUG configuration
#
ifneq ($(SUPPRESS_DEBUG_DEFINE),true)
CXXFLAGS += -DDEBUG
endif

endif

ifeq ($(CONFIG),Release)
CXXFLAGS += -O2 -DNDEBUG
endif

ifeq ($(ASAN),true)
CXXFLAGS += -fsanitize=address
LINKPOSTFIX += -fsanitize=address
endif

ifneq ($(COMPILERSETUP),true)
$(error the compiler named '$(COMPILER)' is not known)
endif
