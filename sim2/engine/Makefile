TOPDIR=../..

include $(TOPDIR)/makefiles/defaults.mk

SOURCES = \
	./cpp/EventsManager.cpp\
	./cpp/ModelFactory.cpp\
	./cpp/SimulationModel.cpp\
	./cpp/SimulationProperties.cpp\
	./cpp/SimulatorEngine.cpp\
	./cpp/SimulatorMessages.cpp\
	./cpp/CTREManager.cpp\
	./cpp/REVManager.cpp\
	./cpp/SimulatedMotor.cpp


TARGET=engine

NEED_WPIUTIL=true
NEED_NTCORE=true
NEED_WPILIB=true
NEED_HAL=true
NEED_XEROBASE=true
NEED_XEROMISC=true

LOCAL_CFLAGS += -Iinclude -Iinclude/ctre

include $(TOPDIR)/makefiles/buildlib.mk
