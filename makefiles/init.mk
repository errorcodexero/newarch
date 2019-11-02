
#
# Make sure the CONFIG is valid
#
ifeq ($(CONFIG),ASan)
CONFIG=Debug
ASAN=true
endif

ifeq ($(CONFIG),Debug)
else
ifeq ($(CONFIG),Release)
else
$(error CONFIG must be set to 'Debug' or 'Release')
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

ifneq ($(PLATFORMVALID),true)
$(error PLATFORM must be set to 'PI' or 'GOPIGO' or 'SIMULATOR')
endif

UNAME := $(shell uname)
ifeq ($(UNAME),Darwin)
MYOS=Darwin
endif
ifeq ($(UNAME),Linux)
MYOS=Linux
endif
ifeq ($(UNAME),CYGWIN_NT-10.0)
MYOS=Windows
endif

ifeq ($(MYOS),)
$(error Could not detect operating system)
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

