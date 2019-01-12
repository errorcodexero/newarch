ifeq ($(CONFIG),Debug)
else
ifeq ($(CONFIG),Release)
else
$(error CONFIG must be set to 'Debug' or 'Release')
endif
endif

ifeq ($(PLATFORM),PI)
else
ifeq ($(PLATFORM),GOPIGO)
else
ifeq ($(PLATFORM),CYGWIN)
else
$(error PLATFORM must be set to 'PI' or 'GOPIGO' or 'CYGWIN')
endif
endif
endif
