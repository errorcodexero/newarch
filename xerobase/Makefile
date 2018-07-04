CC=../../../external/frc/bin/arm-frc-linux-gnueabi-g++
AR=../../../external/frc/bin/arm-frc-linux-gnueabi-ar

CFLAGS = -std=c++1y -Wall -O0 -Og -g3 -fmessage-length=0 -pthread
CINCS = \
	-I../../../external/wpilib/cpp/current/include

TARGET=xerobase.a

CONFIG=Debug

SRC = \
	Drivebase.cpp\
	Robot.cpp

OBJS = $(addprefix $(CONFIG)/,$(SRC:.cpp=.o))

all: mkdirs $(CONFIG)/$(TARGET)

$(CONFIG)/$(TARGET): $(OBJS)
	$(AR) rs $@ $?

$(CONFIG)/%.o: %.cpp
	$(CC) -c $(CFLAGS) $(CINCS) $< -o $@

mkdirs:
	mkdir -p $(CONFIG)

.PHONY: mkdirs
