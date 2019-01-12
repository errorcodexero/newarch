#
# Setup the compilers
#

ifeq ($(PLATFORM),PI)
CXX = arm-linux-gnueabihf-g++
CXXFLAGS=-g $(LOCAL_CFLAGS) -Wno-psabi
endif

ifeq ($(PLATFORM),GOPIGO)
CXX = arm-linux-gnueabihf-g++
CXXFLAGS=-g $(LOCAL_CFLAGS) -Wno-psabi -DGOPIGO
endif

ifeq ($(PLATFORM),CYGWIN)
CXX = g++
CXXFLAGS = -g $(LOCAL_CFLAGS) -DCYGWIN
endif

#
# Add
#
include $(TOPDIR)/makefiles/addons.mk


#
# Define a macro to launch the compiler given an object file and its source
#
define BUILD_C_FILE

#
# Include depenecy information that has been generated
#
-include $$(subst .o,.d,$(2))

$(2): $(1)
	@mkdir -p $$(dir $$@)
ifdef VERBOSE
	$(CXX) -c $(CXXFLAGS) -MD -MP -MF $$(@:.o=.d) -o $$@ $$<
else
	@echo "    Building file $$<"
	@$(CXX) -c $(CXXFLAGS) -MD -MP -MF $$(@:.o=.d) -o $$@ $$<
endif
endef




