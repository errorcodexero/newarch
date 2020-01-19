TOPDIR=../..

SOURCES = \
	./cpp/EventsManager.cpp\
	./cpp/ModelFactory.cpp\
	./cpp/SimulationModel.cpp\
	./cpp/SimulationProperties.cpp\
	./cpp/SimulatorEngine.cpp\
	./cpp/SimulatorMessages.cpp\
	./cpp/CTREManager.cpp


TARGET=engine

NEED_WPIUTIL=true
NEED_NTCORE=true
NEED_WPILIB=true
NEED_HAL=true

LOCAL_CFLAGS += -Iinclude -Iinclude/ctre

include $(TOPDIR)/makefiles/buildlib.mk
