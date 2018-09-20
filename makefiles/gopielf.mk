CXX = arm-linux-gnueabihf-g++

CXXINCS = \
	-I../gopigo3hw\
	-I../gopigo3frc\
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
	../gopigo3navx/gopigo3navx.a\
	../gopigo3frc/gopigo3frc.a\
	../gopigo3hw/gopigo3hw.a\


$(TARGET) : $(PILIBS) $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS) $(PILIBS) -lwiringPI -lpthread

DEPLOYFILES = $(TARGET) $(DEPLOY_EXTRA)

deploy: $(TARGET)
ifdef GOPIGOIP
	scp $(DEPLOYFILES) pi@$(GOPIGOIP):/home/pi
else
	echo GOPIGOIP is not defined
endif

run:
	ssh -t pi@$(GOPIGOIP) "./$(TARGET) --start 0 --auto 15 --oper 0"


clean:
	rm -f $(OBJECTS)
ifdef NEED_XERO
	(cd ../../xerobase ; make clean)
	(cd ../../xeromisc ; make clean)
	(cd ../../xeromath ; make clean)
endif
	(cd ../gopigo3navx ; make clean)
	(cd ../gopigo3frc ; make clean)
	(cd ../gopigo3hw ; make clean)

ifdef NEED_XERO
../../xerobase/gopigo3xerobase.a:
	(cd ../../xerobase ; make)

../../xeromisc/gopigo3xeromisc.a:
	(cd ../../xeromisc ; make)

../../xeromath/gopigo3xeromath.a:
	(cd ../../xeromath ; make)
endif

../gopigo3navx/gopigo3navx.a:
	(cd ../gopigo3navx ; make)

../gopigo3frc/gopigo3frc.a:
	(cd ../gopigo3frc ; make)

../gopigo3hw/gopigo3hw.a:
	(cd ../gopigo3hw ; make)

.PHONY: $(PILIBS)


