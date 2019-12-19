#
# Check that the CONFIG and PLATFORM variable is set correctly and
# perform other misc initialization
#
include $(TOPDIR)/makefiles/init.mk

#
# Define the build directory and associated paths
#
include $(TOPDIR)/makefiles/paths.mk

#
# include the compiler file (initially defines compiler/linker flags)
#
include $(TOPDIR)/makefiles/compiler.mk

#
# include the platform file (adds to compiler/linker flags)
#
include $(TOPDIR)/makefiles/platform.mk

#
# include the macros needed to build
#
include $(TOPDIR)/makefiles/buildmacros.mk

#
# Add on any standard library support, WPILIB, WPIUTIL, CTRE, NAVX (adds to compiler/linker flags)
#
include $(TOPDIR)/makefiles/addons.mk

#
# Define the build target file
#
TARGETFILE=$(BUILDTARGETDIR)/$(TARGET).a

#
# Create the list of object files needed in the build directory
#
OBJECTS = $(addprefix $(BUILDTARGETDIR)/,$(SOURCES:.cpp=.o))

#
# Target to build the library
#
buildlib: post

pre:
	@echo Building library target $(TARGET).a

mkdirs: pre
	$(QUIET)mkdir -p $(BUILDTARGETDIR)

post: $(TARGETFILE)
	@echo Library target $(TARGET).a is complete

$(TARGETFILE): mkdirs $(OBJECTS)
	$(QUIET)$(AR) rvs $@ $(OBJECTS) $(CMDTERM)

clean:
	@echo Cleaning library target $(TARGET).a
	$(QUIET)rm -rf $(OBJECTS) $(TARGETFILE)

$(foreach srcfile,$(SOURCES),$(eval $(call BUILD_C_FILE,$(srcfile),$(BUILDTARGETDIR)/$(srcfile:%.cpp=%.o))))
