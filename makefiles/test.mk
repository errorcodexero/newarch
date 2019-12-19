ifndef CONFIG
CONFIG=DEBUG
endif

PLATFORM=SIMULATOR

OBJFILES=$(subst .cpp,.o,$(TESTFILES))

CATCH2DIR = $(TOPDIR)/external/catch

MYDIR=$(notdir $(PWD))
BASENAME=$(subst test,,$(MYDIR))

CPPFLAGS = -I../$(BASENAME) -I$(CATCH2DIR) -std=gnu++11 $(LOCALFLAGS)
XEROMISCLIBS=$(TOPDIR)/makebuild/$(PLATFORM)/$(CONFIG)/$(BASENAME)/$(BASENAME).a

all: $(MYDIR)

$(MYDIR): $(OBJFILES)
	gcc -o $@ $(OBJFILES) $(XEROMISCLIBS) $(GTESTLIBS) -lstdc++ -lm

runtest:
	./$(MYDIR)
