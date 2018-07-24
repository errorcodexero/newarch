TESTFILES = \
	PIDCtrlTest.cpp

OBJFILES=$(subst .cpp,.o,$(TESTFILES))

ifndef GTESTLIBDIR
GTESTLIBDIR=../external/build/googletest
endif

ifndef GTESTINCDIR
GTESTINCDIR=../external/googletest/googletest/include
endif

CPPFLAGS=-I$(GTESTINCDIR) -I../xeromisc


all: $(OBJFILES)
	gcc -o $(OBJFILES) -L$(GTESTLIBDIR) -lgtest_main -lgtest


