#
#
#

TESTDIRS=xeromisctest
OPENPAREN=(
CLOSEPAREN=)
SEMICOLON=;
SPACE=
SPACE+= 
RUNCMD=$(subst _,$(SPACE),$(addsuffix _$(SEMICOLON)_make_runtest$(CLOSEPAREN)_$(SEMICOLON)_,$(addprefix $(OPENPAREN)cd_,$(TESTDIRS))))

all: runtests

runtests: $(TESTDIRS)
	($(RUNCMD))

xeromisctest:
	(cd xeromisctest ; make)

googletest:
	mkdir -p external/build/googletest
	(cd external/build/googletest ; cmake -G "Unix Makefiles" ../../googletest/googletest ; make)


.PHONY: xeromisctest
