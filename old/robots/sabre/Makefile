#
# Makefile for building the SABRE repository.  This is used by jenkins to
# do periodic builds
#

WPILIB=/usr/local/wpilib
TARGET = sabre

all: $(TARGET)

$(TARGET)::
	(cd util ; make)
	(cd input ; make)
	(cd control ; make)
	(cd modes ; make)
	(cd roborio ; make WPILIB=$(WPILIB) TARGET=$(TARGET))

clean::
	rm */*.o */*.a

