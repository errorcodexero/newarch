#
# Makefile for the vision application running on the Raspberry Pi
#

#
# define the target executable
#
TARGET=VisionCubeDetector

#
# Tell the make system we want the shared code (COMMON)
#
#COMMON=helpers

#
# Tell the make system we need to the WPI libraries
#
WPILIB=false

#
# Tell the make system we need the NAVX libraries
#
NAVX=false

#
# Tell the make system we need the CTRE libraries
#
CTRE=false

#
# Add build flags and libraries
#
CXXFLAGS = `pkg-config --cflags opencv`
ADDLIBS = `pkg-config --libs opencv libv4l2`


#
# define the app lib
#
APPLIBS=

#
# define the source code
#
SRC = \
	Cube.cpp\
	params_parser.cpp\
	main.cpp

#
# On Linux x86, build using native compiler instead of ARM cross-compiler.
# Enable development on Ubuntu.  Final version built and deployed on Raspberry Pi.
# This program will not run on the roboRIO.
#
ifeq ($(shell uname -a | awk '{print $$1$$12}') ,Linuxx86_64)
	USE_NATIVE_COMPILER := yes
endif


include ../../../makefiles/compileapp.mk
