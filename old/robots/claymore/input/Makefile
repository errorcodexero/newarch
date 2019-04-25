#
# makefile for the control library
#

#
# define our target library
#
TARGET=input.a

COMMON=helpers

#
# define the source code files
#
SRC = \
	panel.cpp\
	util.cpp

ifndef TESTS
TESTS = \
	util
endif

ifndef COMPLEXTESTS
COMPLEXTESTS = \
	panel
endif


#
# include a central make file that defines our compilation recipe
#
include ../../../makefiles/compilelib.mk

$(TESTTARGETDIR)/panel$(EXEEXT): panel.cpp util.cpp
