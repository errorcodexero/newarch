#
# makefile for the control library
#

#
# define our target library
#
TARGET=control.a

#
# Tell the make system we want the shared code (COMMON)
#
COMMON=helpers

#
# Tell the make system we want WPILIB headers
#
WPILIB=true

#
# Add flags to indicate the robot we are building
#
ADDCXXFLAGS = -DTUNING -DCLAYMORE -I../util -I../executive -I../input -I. -I..

#
# define the source code files
#
SRC = \
	drivebase.cpp\
	drivebase_controller.cpp\
	force.cpp\
	force_interface.cpp\
	grabber.cpp\
	grabber_controller.cpp\
	intake.cpp\
	lifter.cpp\
	lifter_controller.cpp\
	lights.cpp\
	log.cpp\
	main.cpp\
	monitor.cpp\
	motor_check.cpp\
	nop.cpp\
	pump.cpp\
	sim.cpp\
	toplevel.cpp\
	wings.cpp

ifndef TESTS
TESTS = \
	motor_check
endif

ifndef COMPLEXTESTS
COMPLEXTESTS = \
	lights\
	drivebase\
	force_interface\
	grabber\
	intake\
	lifter\
	log\
	main\
	monitor\
	nop\
	pump\
	toplevel\
	wings
endif

#
# include a central make file that defines our compilation recipe
#
include ../../../makefiles/compilelib.mk

$(TESTTARGETDIR)/drivebase$(EXEEXT): drivebase.cpp motor_check.cpp drivebase_controller.cpp
$(TESTTARGETDIR)/drivebase$(EXEEXT): ../util/countdown_timer.cpp ../util/stall_monitor.cpp ../util/interface.cpp 
$(TESTTARGETDIR)/drivebase$(EXEEXT): ../util/util.cpp ../util/point.cpp ../util/driver_station_interface.cpp

$(TESTTARGETDIR)/force_interface$(EXEEXT): force_interface.cpp force.cpp
$(TESTTARGETDIR)/force_interface$(EXEEXT): ../util/interface.cpp ../util/driver_station_interface.cpp
$(TESTTARGETDIR)/force_interface$(EXEEXT): ../util/posedge_trigger.cpp ../util/util.cpp

$(TESTTARGETDIR)/grabber$(EXEEXT): grabber.cpp nop.cpp
$(TESTTARGETDIR)/grabber$(EXEEXT): ../util/interface.cpp ../util/driver_station_interface.cpp ../util/countdown_timer.cpp ../util/util.cpp

$(TESTTARGETDIR)/intake$(EXEEXT): intake.cpp nop.cpp
$(TESTTARGETDIR)/intake$(EXEEXT): ../util/interface.cpp ../util/driver_station_interface.cpp ../util/util.cpp

$(TESTTARGETDIR)/lifter$(EXEEXT): lifter.cpp
$(TESTTARGETDIR)/lifter$(EXEEXT): ../util/interface.cpp ../util/driver_station_interface.cpp ../util/util.cpp

$(TESTTARGETDIR)/lights$(EXEEXT): lights.cpp nop.cpp
$(TESTTARGETDIR)/lights$(EXEEXT): ../util/interface.cpp ../util/driver_station_interface.cpp ../util/util.cpp

$(TESTTARGETDIR)/log$(EXEEXT): log.cpp nop.cpp toplevel.cpp drivebase.cpp pump.cpp lifter.cpp lights.cpp intake.cpp grabber.cpp motor_check.cpp
$(TESTTARGETDIR)/log$(EXEEXT): ../util/interface.cpp ../util/driver_station_interface.cpp ../util/util.cpp ../util/countdown_timer.cpp ../util/speed_tracker.cpp
$(TESTTARGETDIR)/log$(EXEEXT): ../util/stall_monitor.cpp ../util/countdown_timer.cpp

$(TESTTARGETDIR)/main$(EXEEXT): main.cpp toplevel.cpp nop.cpp pump.cpp drivebase.cpp intake.cpp grabber.cpp nop.cpp lifter.cpp lights.cpp
$(TESTTARGETDIR)/main$(EXEEXT): motor_check.cpp force_interface.cpp force.cpp log.cpp
$(TESTTARGETDIR)/main$(EXEEXT): ../util/countup_timer.cpp ../util/speed_tracker.cpp ../util/countdown_timer.cpp ../util/stall_monitor.cpp
$(TESTTARGETDIR)/main$(EXEEXT): ../util/interface.cpp ../util/posedge_trigger.cpp ../util/driver_station_interface.cpp
$(TESTTARGETDIR)/main$(EXEEXT): ../util/perf_tracker.cpp ../util/util.cpp ../util/type.cpp
$(TESTTARGETDIR)/main$(EXEEXT): ../util/motion_profile.cpp
$(TESTTARGETDIR)/main$(EXEEXT): ../executive/executive.cpp ../executive/teleop.cpp ../executive/autonomous.cpp
$(TESTTARGETDIR)/main$(EXEEXT): ../executive/chain.cpp ../executive/step.cpp
$(TESTTARGETDIR)/main$(EXEEXT): ../input/panel.cpp ../input/util.cpp

$(TESTTARGETDIR)/nop$(EXEEXT): nop.cpp
$(TESTTARGETDIR)/nop$(EXEEXT): ../util/interface.cpp ../util/driver_station_interface.cpp ../util/util.cpp

$(TESTTARGETDIR)/monitor$(EXEEXT): monitor.cpp force_interface.cpp toplevel.cpp force.cpp
$(TESTTARGETDIR)/monitor$(EXEEXT): pump.cpp drivebase.cpp lifter.cpp lights.cpp intake.cpp grabber.cpp nop.cpp motor_check.cpp
$(TESTTARGETDIR)/monitor$(EXEEXT): ../util/interface.cpp ../util/driver_station_interface.cpp ../util/util.cpp
$(TESTTARGETDIR)/monitor$(EXEEXT): ../util/posedge_trigger.cpp ../util/stall_monitor.cpp ../util/speed_tracker.cpp ../util/countdown_timer.cpp

$(TESTTARGETDIR)/pump$(EXEEXT): pump.cpp
$(TESTTARGETDIR)/pump$(EXEEXT): ../util/interface.cpp ../util/driver_station_interface.cpp ../util/util.cpp

$(TESTTARGETDIR)/toplevel$(EXEEXT): toplevel.cpp drivebase.cpp lifter.cpp lifter_brake.cpp lights.cpp grabber.cpp intake.cpp wings.cpp pump.cpp motor_check.cpp nop.cpp
$(TESTTARGETDIR)/toplevel$(EXEEXT): ../util/speed_tracker.cpp ../util/interface.cpp ../util/driver_station_interface.cpp ../util/util.cpp
$(TESTTARGETDIR)/toplevel$(EXEEXT): ../util/countdown_timer.cpp ../util/stall_monitor.cpp ../util/input.cpp
$(TESTTARGETDIR)/toplevel$(EXEEXT): ../util/output.cpp

$(TESTTARGETDIR)/wings$(EXEEXT): wings.cpp nop.cpp
$(TESTTARGETDIR)/wings$(EXEEXT): ../util/interface.cpp ../util/driver_station_interface.cpp ../util/util.cpp
