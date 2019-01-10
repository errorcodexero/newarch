include $(TOPDIR)/gopigo3/makefiles/gopigobuild.mk

ifeq ($(CONFIG),)
$(error no configuration set, must set CONFIG to Debug or Release)
endif

ifeq ($(BUILD),)
BUILD=$(TOPDIR)/gopigo3/build
endif

ifeq ($(CONFIG),Debug)
CXXFLAGS=-DXERODEBUG
endif

ifeq ($(CONFIG),Release)
endif

BUILDDIR=$(BUILD)/$(CONFIG)/$(TARGET)
BUILDTARGET=$(BUILDDIR)/$(TARGET)

CXXINCS = \
	-I../gopigo3hw/include\
	-I../gopigo3hal/include\
	-I../gopigo3nt/include\
	-I../gopigo3wpiutil/include\
	-I../gopigo3wpilib/include\
	-I../gopigo3ctre/include\
	-I../gopigo3navx\
	-I../gopigo3cs/include\
	-I../gopigo3camera/include\
	-I../opencv/include
	-I../../xeromath\
	-I../../xeromisc\
	-I../../xerobase

ROBOTLIBS = \
	$(BUILDDIR)/xerobase/xerobase.a\
	$(BUILDDIR)/xeromisc/xeromisc.a\
	$(BUILDDIR)/xeromath/xeromath.a\
	$(BUILDDIR)/cs/cs.a\
	$(BUILDDIR)/camera/camera.a\
	$(BUILDIDR)/ctre/ctre.a\
	$(BUILDDIR)/navx/navx.a\
	$(BUILDDIR)/wpilib/wpilib.a\
	$(BUILDDIR)/wpiutil/wpiutil.a\
	$(BUILDDIR)/hw/hw.a

CXX = arm-linux-gnueabihf-g++
CXXFLAGS = -g $(LOCAL_CFLAGS) -Wno-psabi -DGOPIGO $(CXXINCS) -DDEBUG
OBJECTS = $(addprefix $(BUILDDIR)/,$(SOURCES:.cpp=.o))

ROBOTLIBS=

$(BUILDTARGET) : $(OBJECTS)
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS) $(ROBOTLIBS) -lwiringPI -lpthread

clean:
	(cd $(TOPDIR)/xerobase ; make CONFIG=$(CONFIG) BUILD=$(BUILD) clean)
	(cd $(TOPDIR)/xermmisc ; make CONFIG=$(CONFIG) BUILD=$(BUILD) clean)
	(cd $(TOPDIR)/xeromath ; make CONFIG=$(CONFIG) BUILD=$(BUILD) clean)
	(cd $(TOPDIR)/gopigo3cs ; make CONFIG=$(CONFIG) BUILD=$(BUILD) clean)
	(cd $(TOPDIR)/gopigi3camera ; make CONFIG=$(CONFIG) BUILD=$(BUILD) clean)
	(cd $(TOPDIR)/gopigo3ctre ; make CONFIG=$(CONFIG) BUILD=$(BUILD) clean)
	(cd $(TOPDIR)/gopigo3navx ; make CONFIG=$(CONFIG) BUILD=$(BUILD) clean)
	(cd $(TOPDIR)/xerobase ; make CONFIG=$(CONFIG) BUILD=$(BUILD) clean)
	(cd $(TOPDIR)/xerobase ; make CONFIG=$(CONFIG) BUILD=$(BUILD) clean)
	(cd $(TOPDIR)/xerobase ; make CONFIG=$(CONFIG) BUILD=$(BUILD) clean)									


#
# Libraries to copy over to the raspberry pi with the deploylibs target.  This is a one time
# action that is not part of every build
#
DEPLOYLIBS=$(wildcard $(TOPDIR)/gopigo3/gopigo3nt/libs) $(wildcard $(TOPDIR)/gopigo3/opencv/libs)

#
# The file to deploy to the raspberry pi on every deploy with every build
#
DEPLOYFILES = $(TARGET) $(DEPLOY_EXTRA)

deploy: $(TARGET)
ifdef GOPIGOIP
	scp $(DEPLOYFILES) pi@$(GOPIGOIP):/home/pi
else
	@echo GOPIGOIP is not defined
endif

deploylibs:
ifdef GOPIGOIP
	ssh -t pi@$(GOPIGOIP) 'mkdir ~/libxfer'
	scp $(DEPLOYLIBS) 'pi@$(GOPIGOIP):~/libxfer'
	ssh -t pi@$(GOPIGOIP) 'sudo cp ~/libxfer/* /usr/lib ; rm -rf ~/libxfer'
else
	@echo GOPIGOIP is not defined
endif

deploydata:
ifdef GOPIGOIP
	scp $(DEPLOY_EXTRA) pi@$(GOPIGOIP):/home/pi
else
	@echo GOPIGOIP is not defined
endif

run:
ifdef GOPIGOIP
	ssh -t pi@$(GOPIGOIP) "./$(TARGET) --station"
else
	echo GOPIGOIP is not defined
endif

runauto:
ifdef GOPIGOIP
	ssh -t pi@$(GOPIGOIP) "./$(TARGET) --auto 25 --oper 0.0 --start 0.0"
else
	echo GOPIGOIP is not defined
endif

debug:
ifdef GOPIGOIP
	ssh -t pi@$(GOPIGOIP) "gdbserver --once 0.0.0.0:3333 GoPiGo3Xero --station"
else
	echo GOPIGOIP is not defined
endif

debugauto:
ifdef GOPIGOIP
	ssh -t pi@$(GOPIGOIP) "gdbserver --once 0.0.0.0:3333 GoPiGo3Xero --auto 25 --oper 0 --start 0"
else
	echo GOPIGOIP is not defined
endif

.PHONY: $(PILIBS)


