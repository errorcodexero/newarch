
# makefile for the executive library
#

#
# define our target library
#
TARGET=executive.a

#
# Tell the make system we want the shared code (COMMON)
#
COMMON=helpers


#
# Add flags to indicate the robot we are building
#
ADDCXXFLAGS = -DCLAYMORE  -I../control -I../util -I../input -I..

ifdef AUTOMODE
ifeq ($(CONFIG),release)
$(info Warning: configuration is release, AUTOMODE is ignored)
else
ADDCXXFLAGS += -DAUTOMODE=$(AUTOMODE)
endif
endif

ifdef AUTOMODE_TEST
$(info AUTOMODE_TEST is no longer supported)
$(info use AUTOMODE=number on the make file for automodes)
$(info see the function get_auto_mode in the file autonomous.cpp for more details)
$(error aboring make)
endif

#
# define our source code
#

DEBUG_AUTOMODES = \
	auto_rotate_pos90.cpp\
	auto_rotate_neg90.cpp\
	auto_rotate_both.cpp\
	auto_drive_straight_long.cpp\
	auto_drive_straight_120.cpp\
	auto_drive_straight_12.cpp\
	auto_drive_straight_neg30.cpp\
	auto_lifter_test.cpp\
	auto_lifter_move.cpp\
	auto_grabber_test.cpp\
	auto_lift_eject.cpp\
	auto_collect_test.cpp\
	auto_lifter_fail_test.cpp\
	auto_curve_test.cpp\
	auto_tune.cpp

COMPETITION_AUTOMODES = \
	auto_cross_line.cpp\
	auto_r_scale_r.cpp\
	auto_r_scale_l.cpp\
	auto_l_scale_r.cpp\
	auto_l_scale_l.cpp\
	auto_c_switch_r.cpp\
	auto_c_switch_l.cpp\
	auto_l_switch_l.cpp\
	auto_r_switch_r.cpp


SRC = \
	autonomous.cpp\
	chain.cpp\
	executive.cpp\
	step.cpp\
	teleop.cpp\
	test.cpp\
	timed.cpp\
	auto_calibonly.cpp\
	$(DEBUG_AUTOMODES)\
	$(COMPETITION_AUTOMODES)

ifndef TESTS
TESTS = \
	timed
endif

ifndef COMPLEXTESTS
COMPLEXTESTS = \
	autonomous\
	chain\
	executive\
	step\
	teleop\
	test
endif

#
# include a central make file that defines our compilation recipe
#
include ../../../makefiles/compilelib.mk


$(TESTTARGETDIR)/autonomous$(EXEEXT): autonomous.cpp teleop.cpp step.cpp executive.cpp chain.cpp test.cpp
$(TESTTARGETDIR)/autonomous$(EXEEXT): ../control/drivebase.cpp ../control/toplevel.cpp ../control/lifter.cpp ../control/lights.cpp ../control/gear_shifter.cpp ../control/intake.cpp ../control/grabber.cpp
$(TESTTARGETDIR)/autonomous$(EXEEXT): ../control/pump.cpp ../control/nop.cpp ../control/motor_check.cpp
$(TESTTARGETDIR)/autonomous$(EXEEXT): ../util/countdown_timer.cpp ../util/speed_tracker.cpp ../util/stall_monitor.cpp ../util/interface.cpp
$(TESTTARGETDIR)/autonomous$(EXEEXT): ../util/driver_station_interface.cpp ../util/motion_profile.cpp ../util/posedge_trigger.cpp
$(TESTTARGETDIR)/autonomous$(EXEEXT): ../util/type.cpp ../util/util.cpp
$(TESTTARGETDIR)/autonomous$(EXEEXT): ../input/util.cpp ../input/panel.cpp

$(TESTTARGETDIR)/chain$(EXEEXT): chain.cpp executive.cpp teleop.cpp step.cpp autonomous.cpp
$(TESTTARGETDIR)/chain$(EXEEXT): ../util/interface.cpp ../util/countdown_timer.cpp ../util/util.cpp
$(TESTTARGETDIR)/chain$(EXEEXT): ../util/stall_monitor.cpp ../util/type.cpp ../util/speed_tracker.cpp
$(TESTTARGETDIR)/chain$(EXEEXT): ../util/driver_station_interface.cpp ../util/posedge_trigger.cpp ../util/motion_profile.cpp
$(TESTTARGETDIR)/chain$(EXEEXT): ../control/toplevel.cpp ../control/lifter.cpp ../control/lights.cpp ../control/pump.cpp ../control/gear_shifter.cpp ../control/intake.cpp ../control/grabber.cpp
$(TESTTARGETDIR)/chain$(EXEEXT): ../control/drivebase.cpp ../control/nop.cpp ../control/motor_check.cpp
$(TESTTARGETDIR)/chain$(EXEEXT): ../input/panel.cpp ../input/util.cpp

$(TESTTARGETDIR)/executive$(EXEEXT): executive.cpp
$(TESTTARGETDIR)/executive$(EXEEXT): ../util/interface.cpp ../util/countdown_timer.cpp ../util/util.cpp
$(TESTTARGETDIR)/executive$(EXEEXT): ../util/stall_monitor.cpp ../util/type.cpp
$(TESTTARGETDIR)/executive$(EXEEXT): ../util/driver_station_interface.cpp ../util/posedge_trigger.cpp ../util/speed_tracker.cpp ../util/motion_profile.cpp
$(TESTTARGETDIR)/executive$(EXEEXT): ../control/toplevel.cpp ../control/lifter.cpp ../control/lights.cpp ../control/pump.cpp ../control/gear_shifter.cpp ../control/intake.cpp ../control/.grabbercpp
$(TESTTARGETDIR)/executive$(EXEEXT): ../control/drivebase.cpp ../control/nop.cpp ../control/motor_check.cpp
$(TESTTARGETDIR)/executive$(EXEEXT): ../input/panel.cpp ../input/util.cpp

$(TESTTARGETDIR)/step$(EXEEXT): step.cpp
$(TESTTARGETDIR)/step$(EXEEXT): ../util/interface.cpp ../util/countdown_timer.cpp ../util/util.cpp
$(TESTTARGETDIR)/step$(EXEEXT): ../util/stall_monitor.cpp ../util/type.cpp
$(TESTTARGETDIR)/step$(EXEEXT): ../util/driver_station_interface.cpp ../util/posedge_trigger.cpp ../util/speed_tracker.cpp ../util/motion_profile.cpp
$(TESTTARGETDIR)/step$(EXEEXT): ../control/toplevel.cpp ../control/lifter.cpp ../control/lights.cpp ../control/pump.cpp ../control/gear_shifter.cpp ../control/intake.cpp ../control/grabber.cpp
$(TESTTARGETDIR)/step$(EXEEXT): ../control/drivebase.cpp ../control/nop.cpp ../control/motor_check.cpp
$(TESTTARGETDIR)/step$(EXEEXT): ../input/panel.cpp ../input/util.cpp


$(TESTTARGETDIR)/teleop$(EXEEXT): teleop.cpp executive.cpp test.cpp autonomous.cpp step.cpp chain.cpp
$(TESTTARGETDIR)/teleop$(EXEEXT): ../util/interface.cpp ../util/countdown_timer.cpp ../util/speed_tracker.cpp ../util/util.cpp
$(TESTTARGETDIR)/teleop$(EXEEXT): ../util/stall_monitor.cpp ../util/type.cpp
$(TESTTARGETDIR)/teleop$(EXEEXT): ../util/driver_station_interface.cpp ../util/posedge_trigger.cpp ../util/motion_profile.cpp
$(TESTTARGETDIR)/teleop$(EXEEXT): ../control/toplevel.cpp ../control/lifter.cpp ../control/lights.cpp ../control/pump.cpp ../control/gear_shifter.cpp ../control/intake.cpp ../control/grabber.cpp
$(TESTTARGETDIR)/teleop$(EXEEXT): ../control/drivebase.cpp ../control/nop.cpp ../control/motor_check.cpp
$(TESTTARGETDIR)/teleop$(EXEEXT): ../input/panel.cpp ../input/util.cpp

$(TESTTARGETDIR)/test$(EXEEXT): test.cpp executive.cpp teleop.cpp autonomous.cpp step.cpp chain.cpp
$(TESTTARGETDIR)/test$(EXEEXT): ../util/interface.cpp ../util/countdown_timer.cpp ../util/util.cpp
$(TESTTARGETDIR)/test$(EXEEXT): ../util/stall_monitor.cpp ../util/type.cpp
$(TESTTARGETDIR)/test$(EXEEXT): ../util/driver_station_interface.cpp ../util/posedge_trigger.cpp ../util/speed_tracker.cpp ../util/motion_profile.cpp
$(TESTTARGETDIR)/test$(EXEEXT): ../control/toplevel.cpp ../control/lifter.cpp ../control/lights.cpp ../control/pump.cpp ../control/gear_shifter.cpp ../control/intake.cpp ../control/grabber.cpp
$(TESTTARGETDIR)/test$(EXEEXT): ../control/drivebase.cpp ../control/nop.cpp ../control/motor_check.cpp
$(TESTTARGETDIR)/test$(EXEEXT): ../input/panel.cpp ../input/util.cpp

