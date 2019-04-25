#
# makefile for the misc library
#

#
# define our target library
#
TARGET=helpers.a
WPILIB=true
#
# define the source code files
#
SRC = \
	params_parser.cpp\
	udp_sync.cpp\
	message_logger.cpp\
	message_dest_file.cpp\
	message_dest_seq_file.cpp\
	motion_target_profile.cpp\
	motor_current_monitor.cpp\
	data_logger.cpp\
	data_logger_destination_server.cpp

# message_logger.cpp

ifndef TESTS
TESTS = \
	data_logger_destination_server \
	message_dest_file \
	motion_target_profile \
	motor_current_monitor
endif

#	message_logger \


#
# include a central make file that defines our compilation recipe
#
include ../../../makefiles/compilelib.mk
