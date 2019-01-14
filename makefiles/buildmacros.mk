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
