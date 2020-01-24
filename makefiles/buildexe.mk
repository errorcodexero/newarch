
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
TARGETFILE=$(BUILDTARGETDIR)/$(TARGET)$(EXEEXT)

#
# Create the list of object files needed in the build directory
#
OBJECTS = $(addprefix $(BUILDTARGETDIR)/,$(SOURCES:.cpp=.o))

#
# Combine to get complete list of librarys to build and link
#
ALLLIBLIST=$(LOCAL_LIBS) $(LIBLIST)
#
# Generate the flat library list that will be in the build directory
#
ALLFLATLIBLIST=$(notdir $(ALLLIBLIST))

#
# Create the list of libraries from the LIBLIST
#
LINKLIBS=$(foreach linklib,$(ALLFLATLIBLIST),$(BUILDHOME)/$(linklib)/$(linklib).a)

#
# Create the complete list of libraries to link against
#
ALLLIBS=$(LINKLIBS) $(LOCAL_EXTERNAL_LIBS) $(PLATFORMLIBS)
SHORTLIBS=$(foreach libname,$(ALLLIBLIST),$(notdir $(libname)))

ifndef SILENT
$(info $(SPACE)$(SPACE)Libraries: $(SHORTLIBS))
$(info -------------------------------------------------)
endif

PARALLEL=-j

all: buildexe

help:
	@echo Targets ...
	@echo "    all - builds all required libraries and the robot application"
	@echo "    apponly - builds only the robot application, assumes the libraries are already built"
	@echo "    deploy - deploys the application and deploy directory to the target robot (GoPiGO3 only)"

#
# Target to build the library
#
buildexe:
	@make -j --no-print-directory SILENT=true PLATFORM=$(PLATFORM) CONFIG=$(CONFIG) buildlibs
	@make -j --no-print-directory SILENT=true PLATFORM=$(PLATFORM) CONFIG=$(CONFIG) apponly

buildold: buildlibs apponly

apponly: post

pre:
	@echo Building executable file $(TARGET)$(EXEEXT)

mkdirs: pre
	$(QUIET)mkdir -p $(BUILDTARGETDIR)

post: $(TARGETFILE)
	@echo Executable file $(TARGET)$(EXEEXT) complete

$(TARGETFILE): mkdirs $(OBJECTS) $(LINKLIBS)
	@echo Linking ...
	$(QUIET)$(CXX) -g -o $@ $(OBJECTS) $(LINKPREFIX) -Wl,--start-group $(ALLLIBS) -Wl,--end-group $(LINKPOSTFIX)

clean: cleanlibs
	@echo Cleaning executable target $(TARGET)$(EXEEXT)
	$(QUIET)rm -rf $(OBJECTS) $(TARGETFILE)

buildlibs:
	$(QUIET)for libdir in $(ALLLIBLIST) ; do \
		cd $$libdir ; \
		make --no-print-directory SILENT=true PLATFORM=$(PLATFORM) CONFIG=$(CONFIG) NEED_GOPIGO3HW=$(NEED_GOPIGO3HW);\
	done

cleanlibs:
	$(QUIET)for libdir in $(ALLLIBLIST) ; do \
		cd $$libdir ; \
		CONFIG=$(CONFIG) PLATFORM=$(PLATFORM) make SILENT=true --no-print-directory clean ;\
	done

UNAME := $(shell uname)
ifeq ($(MYOS),Darwin)
CYGTARGETFILE=$(realpath $(TARGETFILE))
endif
ifeq ($(MYOS),Linux)
CYGTARGETFILE=$(realpath $(TARGETFILE))
endif
ifeq ($(MYOS),Windows)
CYGTARGETFILE=$(shell cygpath -a -u $(TARGETFILE))
endif

ifeq ($(PLATFORM),GOPIGO)
ifeq ($(GOPIGOIP),)
$(error GOPIGOIP make variable not defined)
endif
deploy:
	scp $(CYGTARGETFILE) pi@$(GOPIGOIP):/home/pi
	if [ -d "deploy" ]; then \
		scp -r deploy pi@$(GOPIGOIP):/home/pi; \
	fi

run:
	ssh pi@$(GOPIGOIP) /home/pi/$(TARGET)$(EXEEXT) --auto 30 --oper 0 --start 0

kill:
	ssh pi@$(GOPIGOIP) pkill -x $(TARGET)$(EXEEXT) || true

allrun:	buildexe kill deploy run
endif

ifeq ($(PLATFORM),SIMULATOR)
deploy:
	$(QUIET)if [ -d "src/main/deploy" ]; then \
		mkdir -p $(TOPDIR)/deploy/$(TARGET) ;\
		cp -r src/main/deploy $(TOPDIR)/deploy/$(TARGET) ; \
	fi
	$(QUIET)echo Deploy complete	
endif

ifeq ($(PLATFORM),SIM2)
deploy:
	$(QUIET)if [ -d "src/main/deploy" ]; then \
		mkdir -p $(TOPDIR)/deploy/$(TARGET) ;\
		cp -r src/main/deploy $(TOPDIR)/deploy/$(TARGET) ; \
	fi
	$(QUIET)echo Deploy complete
endif

.PHONY: deploy run kill allrun

$(foreach srcfile,$(SOURCES),$(eval $(call BUILD_C_FILE,$(srcfile),$(BUILDTARGETDIR)/$(srcfile:%.cpp=%.o))))
