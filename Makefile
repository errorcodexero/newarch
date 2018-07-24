#
#
#

all: xeromisctest

xeromisctest:
	(cd xeromisctest ; make)

googletest:
	mkdir -p external/build/googletest
	(cd external/build/googletest/googletest ; cmake -G "Unix Makefiles" ../../googletest/googletest ; make)


.PHONY: xeromisctest
