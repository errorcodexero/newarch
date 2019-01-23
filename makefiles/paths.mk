#
# Setup the various paths for the build environment
#

ifndef BUILDDIR
BUILDDIR=$(TOPDIR)/makebuild
endif

#
# Get the real full path
#
REALBUILDDIR=$(shell cygpath -m -a $(BUILDDIR))

#
# This is the home directory for the build tree
#
BUILDHOME=$(REALBUILDDIR)/$(PLATFORM)/$(CONFIG)

#
# This is the build directory for the target
#
BUILDTARGETDIR=$(BUILDHOME)/$(TARGET)

#
# Print the paths we are using
#
ifdef MAKEDEBUG
$(info ----------------------------------------------------------------------------)
$(info BUILDDIR '$(BUILDDIR)')
$(info REALBUILDDIR '$(REALBUILDDIR)')
$(info BUILDHOME '$(BUILDHOME)')
$(info BUILDTARGETDIR '$(BUILDTARGETDIR)')
$(info ----------------------------------------------------------------------------)
endif