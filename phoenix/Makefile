CC=../../../external/frc/bin/arm-frc-linux-gnueabi-g++
AR=../../../external/frc/bin/arm-frc-linux-gnueabi-ar

CFLAGS = -std=c++1y -Wall -O0 -Og -g3 -fmessage-length=0 -pthread
CINCS = \
	-I../xerobase\
	-I../../../external/wpilib/cpp/current/include

TARGET=frcUserProgram

CONFIG=Debug

SRC = \
	Phoenix.cpp

LIBS = \
	../xerobase/$(CONFIG)/xerobase.a\
	-L../../../external/wpilib/common/current/lib/linux/athena/shared\
	-L../../../external/wpilib/cpp/current/reflib/linux/athena/shared\
	-lwpi

OBJS = $(addprefix $(CONFIG)/,$(SRC:.cpp=.o))

all: mkdirs $(CONFIG)/$(TARGET)

$(CONFIG)/$(TARGET): $(OBJS)
	$(CC) -o $@ $(CFLAGS) $(OBJS) $(LIBS)

$(CONFIG)/%.o: %.cpp
	$(CC) -c $(CFLAGS) $(CINCS) $< -o $@

mkdirs:
	mkdir -p $(CONFIG)

.PHONY: mkdirs
