#
# include the validation stuff
#

include ../../makefiles/valid.mk

SUBDIRS = \
	helpers

all:: $(SUBDIRS)

$(SUBDIRS)::
	(cd $@ ; $(MAKE) CONFIG=$(CONFIG))

.PHONY: all $(SUBDIRS)
