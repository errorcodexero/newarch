#
# On Windows, use Pi cross-compiler.  On Pi, use native/default compiler.
# At this time, the Makefile is only set up to cross-compile for ARM on Windows.
# The frc include files and libs have to be copied from the FRCVision image on the Pi to /usr/local/frc.
# If not present, they will be copied via scp from the host PIIP before the build commences.
#

ifeq ($(OS),Windows_NT)
CXX=arm-raspbian9-linux-gnueabihf-g++
CC=${CXX}
endif

#SRC_FILES := $(wildcard *.cpp)
#OBJ_FILES := $(patsubst %.cpp,%.o,$(SRC_FILES))

# Binaries to build
VISION=vision_phaser2019
PLAYER=video_player
RECORDER=video_recorder

BINS = ${VISION} ${PLAYER} ${RECORDER}

DESTDIR?=/home/pi/

.PHONY: clean build install deploy import_extlibs

build: import_extlibs ${BINS}

install: build
	cp ${BINS} runCamera ${DESTDIR}

clean:
	rm -f ${BINS} *.o

LDFLAGS = -pthread \
	-L/cygwin64/usr/local/frc/lib

LDLIBS = -lwpilibc \
	-lwpiHal \
	-lcameraserver \
	-lcscore \
	-lntcore \
	-lwpiutil \
	-lopencv_ml \
	-lopencv_objdetect \
	-lopencv_shape \
	-lopencv_stitching \
	-lopencv_superres \
	-lopencv_videostab \
	-lopencv_calib3d \
	-lopencv_features2d \
	-lopencv_highgui \
	-lopencv_videoio \
	-lopencv_imgcodecs \
	-lopencv_video \
	-lopencv_photo \
	-lopencv_imgproc \
	-lopencv_flann \
	-lopencv_core

${VISION}: ${VISION}.o params_parser.o  #../../xerolibs/xeromisc/SettingsParser.o
${PLAYER}: ${PLAYER}.o
${RECORDER}: ${RECORDER}.o

.cpp.o:
	${CXX} -pthread -O -c -o $@ -I/cygwin64/usr/local/frc/include -I../../xerolibs/xeromisc $<

# PIIP = IP or hostname of the Raspberry Pi

deploy:
ifdef PIIP
	ssh -t pi@${PIIP} 'rm -f ${EXE}'
	#scp runCamera ${BINS} 'pi@${PIIP}:/home/pi'
	scp ${BINS} vision_params.txt 'pi@${PIIP}:/home/pi'
	ssh -t pi@${PIIP} './runService'
else
	@echo PIIP is not defined
endif

import_extlibs:
ifeq (,$(wildcard /usr/local/frc/include))
ifdef PIIP
	mkdir -p /usr/local/frc
	scp -r pi@${PIIP}:/usr/local/frc/include /usr/local/frc
	scp -r pi@${PIIP}:/usr/local/frc/lib /usr/local/frc
else
	@echo PIIP is not defined
endif
endif

