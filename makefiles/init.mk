
#
# Make sure the CONFIG is valid
#
ifeq ($(CONFIG),ASan)
override CONFIG := Debug
ASAN = true
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

ifneq ($(SILENT),true)
SPACE =
SPACE += 
$(info Build Configuration)
$(info $(SPACE)$(SPACE)PLATFORM: $(PLATFORM))
$(info $(SPACE)$(SPACE)CONFIG: $(CONFIG))
$(info $(SPACE)$(SPACE)MYOS: $(MYOS))
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

