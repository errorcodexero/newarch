TOPDIR=..

SOURCES = \
	Point.cpp\
	PointAngle.cpp\
	Polar.cpp\
	QuadraticSolver.cpp

TARGET=xeromath

ifeq ($(PLATFORM),gopigo)
LOCAL_CFLAGS = -I../gopigo3frc
endif

include $(TOPDIR)/makefiles/buildlib.mk

