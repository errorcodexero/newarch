TOPDIR=../..

HALAPIS = \
	cpp/cpp/fpga_clock.cpp\
	cpp/handles/HandlesInternal.cpp\
	cpp/Main.cpp\
	sim2/main.cpp\
	sim2/ports.cpp\
	sim2/report.cpp\
	sim2/analoginput.cpp\
	sim2/analogoutput.cpp\
	sim2/analogtrigger.cpp\
	sim2/error.cpp\
	sim2/accum.cpp\
	sim2/counter.cpp\
	sim2/dio.cpp\
	sim2/ds.cpp\
	sim2/power.cpp\
	sim2/init.cpp\
	sim2/encoder.cpp\
	sim2/i2c.cpp\
	sim2/interrupts.cpp\
	sim2/solenoid.cpp\
	sim2/relay.cpp\
	sim2/serial.cpp\
	sim2/pwm.cpp\
	sim2/spi.cpp\
	sim2/pdp.cpp\
	sim2/notifier.cpp\
	sim2/can.cpp

SIMCLASSES = \
	sim2/engine/SimulatorEngine.cpp

SOURCES = \
	$(HALAPIS)\
	$(SIMCLASSES)

TARGET=hal

NEED_WPIUTIL=true

LOCAL_CFLAGS += -Iinclude -Ibuild -Isim2

include $(TOPDIR)/makefiles/buildlib.mk
