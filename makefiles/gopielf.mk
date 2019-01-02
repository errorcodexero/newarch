CXX = arm-linux-gnueabihf-g++

CXXINCS = \
	-I../gopigo3hw\
	-I../frc\
	-I..\
	-I../gopigo3navx

ifdef NEED_XERO
CXXINCS += \
	-I../../xeromath\
	-I../../xeromisc\
	-I../../xerobase
endif

CXXFLAGS = -g $(LOCAL_CFLAGS) -Wno-psabi -DGOPIGO $(CXXINCS) -DDEBUG

OBJECTS = $(SOURCES:.cpp=.o)

ifdef NEED_XERO
PILIBS += \
	../../xerobase/gopigo3xerobase.a\
	../../xeromisc/gopigo3xeromisc.a\
	../../xeromath/gopigo3xeromath.a
endif

PILIBS += \
	-L../frc/libs\
	../gopigo3navx/gopigo3navx.a\
	../frc/wpilib.a\
	../gopigo3hw/gopigo3hw.a\
	-lntcore\
	-lwpiutil

DEPLOYLIBS=$(wildcard ../frc/libs/*)


$(TARGET) : $(PILIBS) $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS) $(PILIBS) -lwiringPI -lpthread

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


clean:
	rm -f $(OBJECTS)
ifdef NEED_XERO
	bash -c "(cd ../../xerobase ; make clean)"
	bash -c "(cd ../../xeromisc ; make clean)"
	bash -c "(cd ../../xeromath ; make clean)"
endif
	bash -c "(cd ../gopigo3navx ; make clean)"
	bash -c "(cd ../frc ; make clean)"
	bash -c "(cd ../gopigo3hw ; make clean)"

ifdef NEED_XERO
../../xerobase/gopigo3xerobase.a:
	bash -c "(cd ../../xerobase ; make)"

../../xeromisc/gopigo3xeromisc.a:
	bash -c "(cd ../../xeromisc ; make)"

../../xeromath/gopigo3xeromath.a:
	bash -c "(cd ../../xeromath ; make)"
endif

../gopigo3navx/gopigo3navx.a:
	bash -c "(cd ../gopigo3navx ; make)"

../frc/wpilib.a:
	bash -c "(cd ../frc ; make)"

../gopigo3hw/gopigo3hw.a:
	bash -c "(cd ../gopigo3hw ; make)"

.PHONY: $(PILIBS)


