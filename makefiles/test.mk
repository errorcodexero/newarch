
OBJFILES=$(subst .cpp,.o,$(TESTFILES))

ifndef GTESTLIBDIR
GTESTLIBDIR=../external/build/googletest
endif

ifndef GTESTLIBS
GTESTLIBS=-L$(GTESTLIBDIR) -lgtest_main -lgtest
endif

ifndef GTESTINCDIR
GTESTINCDIR=../external/googletest/googletest/include
endif

MYDIR=$(notdir $(PWD))
SIMNAME=$(subst test,sim,$(MYDIR))
BASENAME=$(subst test,,$(MYDIR))

ifdef DEBUG
$(info MYDIR $(MYDIR))
$(info BASENAME $(BASENAME))
$(info SIMNAME $(SIMNAME))
endif

CPPFLAGS = -I../$(BASENAME) -I$(GTESTINCDIR) -std=gnu++11 $(LOCALFLAGS)
XEROMISCLIBS=../build/libs/$(SIMNAME)/static/$(SIMNAME).lib

all: $(MYDIR)

$(MYDIR): $(OBJFILES)
	gcc -o $@ $(OBJFILES) $(XEROMISCLIBS) $(GTESTLIBS) -lstdc++

runtest:
	./$(MYDIR)
