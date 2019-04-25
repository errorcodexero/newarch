
# makefile for the misc library
#

#
# define our target library
#
TARGET=util.a

#
# Tell the make system we want the common code
#
COMMON=helpers

#
# Indicates the robot we are buiding
#
ADDCXXFLAGS = -DCLAYMORE  -I../control -I. -I..

#
# define the source code files
#
SRC = \
	auto_distances.cpp\
	bound_tracker.cpp\
	countdown_timer.cpp\
	countup_timer.cpp\
	debounce.cpp\
	driver_station_interface.cpp\
	fixVictor.cpp\
	input.cpp\
	interface.cpp\
	meta.cpp\
	motion_profile.cpp\
	negedge_trigger.cpp\
	network.cpp\
	output.cpp\
	perf_tracker.cpp\
	persistent_tracker.cpp\
	pidctrl.cpp\
	point.cpp\
	posedge_toggle.cpp\
	posedge_trigger.cpp\
	posedge_trigger_debounce.cpp\
	pwm_transcriber.cpp\
	settable_constant.cpp\
	settable_toggle.cpp\
	speed_tracker.cpp\
	string_utils.cpp\
	type.cpp\
	util.cpp\
	visitable.cpp\
	stall_monitor.cpp\
	side_decoder.cpp

ifndef TESTS
TESTS = \
	auto_distances\
	bound_tracker\
	countdown_timer\
	countup_timer\
	fixVictor\
	input\
	motion_profile\
	negedge_trigger\
	network\
	output\
	perf_tracker\
	pwm_transcriber\
	settable_constant\
	speed_profile\
	stall_monitor\
	string_utils\
	type\
	side_decoder.cpp\
	util
endif

ifndef COMPLEXTESTS
COMPLEXTESTS = \
	debounce\
	visitable\
	speed_tracker\
	settable_toggle\
	posedge_trigger\
	posedge_trigger_debounce\
	point\
	persistent_tracker\
	driver_station_interface\
	interface\
	meta
endif


#
# include a central make file that defines our compilation recipe
#
include ../../../makefiles/compilelib.mk

#
# Complex tests
#
$(TESTTARGETDIR)/debounce$(EXEEXT): debounce.cpp countdown_timer.cpp
$(TESTTARGETDIR)/visitable$(EXEEXT): visitable.cpp type.cpp
$(TESTTARGETDIR)/speed_tracker$(EXEEXT): speed_tracker.cpp countdown_timer.cpp
$(TESTTARGETDIR)/settable_toggle$(EXEEXT): settable_toggle.cpp posedge_trigger.cpp
$(TESTTARGETDIR)/posedge_trigger$(EXEEXT): posedge_trigger.cpp
$(TESTTARGETDIR)/posedge_trigger_debounce$(EXEEXT): posedge_trigger_debounce.cpp countdown_timer.cpp posedge_trigger.cpp
$(TESTTARGETDIR)/point$(EXEEXT): point.cpp util.cpp
$(TESTTARGETDIR)/persistent_tracker$(EXEEXT): persistent_tracker.cpp util.cpp
$(TESTTARGETDIR)/driver_station_interface$(EXEEXT): driver_station_interface.cpp util.cpp
$(TESTTARGETDIR)/interface$(EXEEXT): interface.cpp driver_station_interface.cpp util.cpp
$(TESTTARGETDIR)/meta$(EXEEXT): meta.cpp interface.cpp driver_station_interface.cpp util.cpp


