#
# makefile for the top level robot application
#

#
# define the target executable
#
TARGET=FRCUserProgram

#
# Tell the make system we want the shared code (COMMON)
#
COMMON=helpers

#
# Tell the make system we need to the WPI libraries
#
WPILIB=true

#
# Tell the make system we need the NAVX libraries
#
NAVX=true

#
# Tell the make system we need the CTRE libraries
#
CTRE=true

#
# Add flags to indicate the robot we are building
#
ADDCXXFLAGS = -DCLAYMORE -I../util -I. -I../control -I..

#
# define the app lib
#
APPLIBS=\
	control\
	input\
	executive\
	util

#
# define the source code
#
SRC = \
	dio_control.cpp\
	i2c_control.cpp\
	navx_control.cpp\
	pump_control.cpp\
	talon_srx_control.cpp\
	to_roborio.cpp

include ../../../makefiles/compileapp.mk
