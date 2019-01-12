#
# include the compiler file
#
include $(TOPDIR)/makefiles/compiler.mk

#
# Check that the CONFIG variable is set correctly
#
include $(TOPDIR)/makefiles/config.mk

#
# Define the build directory and associated paths
#
include $(TOPDIR)/makefiles/paths.mk

#
# Define the build target file
#
TARGETFILE=$(BUILDTARGETDIR)/$(TARGET).a

#
# Create the list of object files, with their full paths to
# the 
#
OBJECTS = $(addprefix $(BUILDTARGETDIR)/,$(SOURCES:.cpp=.o))

#
# Target to build the library
#
buildlib: pre mkdirs $(TARGETFILE) post

pre:
	@echo Building library target $(TARGET).a

mkdirs:
	@mkdir -p $(BUILDTARGETDIR)

post:
	@echo Library target $(TARGET).a is complete

$(TARGETFILE): $(OBJECTS)
	@$(AR) rvs $@ $(OBJECTS) > /dev/null 2>&1

clean:
ifdef VERBOSE
	rm -rf $(OBJECTS) $(TARGET).a
else
	@echo Cleaning library target $(TARGET).a
	@rm -rf $(OBJECTS) $(TARGET).a
endif

$(foreach srcfile,$(SOURCES),$(eval $(call BUILD_C_FILE,$(srcfile),$(BUILDTARGETDIR)/$(srcfile:%.cpp=%.o))))
