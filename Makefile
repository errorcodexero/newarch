#
#
#

TESTDIRS=xeromisctest xeromathtest xerobasetest
OPENPAREN=(
CLOSEPAREN=)
SEMICOLON=;
SPACE=
SPACE+= 
RUNCMD=$(subst _,$(SPACE),$(addsuffix _$(SEMICOLON)_make_runtest$(CLOSEPAREN)_$(SEMICOLON)_,$(addprefix $(OPENPAREN)cd_,$(TESTDIRS))))

all:
	@echo Use either 'make unittests' or 'make gopigo'

unittests: build runtests

build:
	./gradlew build

runtests: $(TESTDIRS)
	($(RUNCMD))

xeromisctest:
	(cd xeromisctest ; make)

xeromathtest:
	(cd xeromathtest ; make)

xerobasetest:
	(cd xerobasetest ; make)

googletest:
	mkdir -p external/build/googletest
	(cd external/build/googletest ; cmake -G "Unix Makefiles" ../../googletest/googletest ; make)

clean:
	rm -rf *test/*.o *test/*.exe


gopigo:
	(cd gopigo3hw ; make)
	(cd gopigo3frc ; make)
	(cd gopigo3simple ; make)

.PHONY: $(TESTDIRS) build gopigo

