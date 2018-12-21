SOURCES = \
	GoPiGo3Xero.cpp\
	ServoGoToAngle.cpp\
	ServoSubsystem.cpp\
	GoPiGo3Subsystem.cpp\
	GoPiGo3AutoModeController.cpp\
	GoPiGo3OISubsystem.cpp

TARGET=GoPiGo3Xero

#
# We need the xeromath, xeromisc, and xerobase libraries
#
NEED_XERO=true

#
# When we deploy, copy the robot.dat file as well
#
DEPLOY_EXTRA=robot.dat

include ../../makefiles/gopielf.mk
