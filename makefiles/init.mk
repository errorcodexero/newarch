
#
# Make sure the CONFIG is valid
#
ifeq ($(CONFIG),ASan)
override CONFIG := Debug
ASAN = true
endif

ifeq ($(CONFIG),Debug)
else
ifeq ($(CONFIG),Release)
else
$(info CONFIG not defined, assuming DEBUG)
CONFIG=Debug
endif
endif

#
# Make sure the PLATFORM is valid
#
PLATFORMVALID=

ifeq ($(PLATFORM),PI)
PLATFORMVALID=true
MYCOMPILER=PI
endif

ifeq ($(PLATFORM),GOPIGO)
PLATFORMVALID=true
MYCOMPILER=PI
endif

ifeq ($(PLATFORM),SIMULATOR)
PLATFORMVALID=true
MYCOMPILER=GCC
endif

ifeq ($(PLATFORM),SIM2)
PLATFORMVALID=true
MYCOMPILER=GCC
endif

ifneq ($(PLATFORMVALID),true)
$(info PLATFORM not defined, assuming SIMULATOR)
PLATFORM=SIMULATOR
MYCOMPILER=GCC
endif

UNAME := $(shell uname)
ifeq ($(UNAME),Darwin)
MYOS=Darwin
endif
ifeq ($(UNAME),Linux)
MYOS=Linux
endif
ifeq ($(UNAME),CYGWIN_NT-10.0)
MYOS=Cygwin
endif

ifeq ($(MYOS),)
$(info MYOS not defined, assuming Cygwin)
MYOS=Cygwin
endif


#
# This macros proceeds any action in a make file that we might want to show
# the user in VERBOSE mode
#
ifdef VERBOSE
QUIET=
CMDTERM=
else
QUIET=@
CMDTERM= > /dev/null 2>&1
endif

