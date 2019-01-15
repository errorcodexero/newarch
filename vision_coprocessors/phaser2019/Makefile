#
# On Windows, use Pi cross-compiler.  On Pi, use native/default compiler.
# At this time, the Makefile is only set up to cross-compile for ARM on Windows.
# The frc include files and libs have to be copied from the FRCVision image on the Pi to /usr/local/frc.
# If not present, they will be copied via scp from the host PIIP before the build commences.
#

ifeq ($(OS),Windows_NT)
CXX=arm-raspbian9-linux-gnueabihf-g++
endif

SRC_FILES := $(wildcard *.cpp)
OBJ_FILES := $(patsubst %.cpp,%.o,$(SRC_FILES))

EXE=vision_phaser2019.exe

DESTDIR?=/home/pi/

.PHONY: clean build install deploy import_extlibs

build: import_extlibs ${EXE}

install: build
	cp ${EXE} runCamera ${DESTDIR}

clean:
	rm -f ${EXE} *.o

${EXE}: ${OBJ_FILES}
	${CXX} -pthread -o $@ $< \
	    -L/cygwin64/usr/local/frc/lib \
	    -lwpilibc \
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

.cpp.o:
	${CXX} -pthread -O -c -o $@ -I/cygwin64/usr/local/frc/include $<

# PIIP = IP or hostname of the Raspberry Pi

deploy:
ifdef PIIP
	ssh -t pi@${PIIP} 'rm -f ${EXE}'
	#scp runCamera ${EXE} 'pi@${PIIP}:/home/pi'
	scp ${EXE} 'pi@${PIIP}:/home/pi'
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

