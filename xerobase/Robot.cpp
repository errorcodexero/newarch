#include "Robot.h"
#include "Subsystem.h"
#include "ControllerBase.h"
#include <iostream>
#include <cassert>

namespace xero {
	namespace base {
		
		Robot::Robot() {
			//
			// Set the robot loop time to 50 ms
			//
			target_loop_time_ = 0.05 ;
		}
		
		void Robot::robotLoop() {
			double initial_time = frc::Timer::GetFPGATimestamp();

			for (SubsystemPtr& subsystem : subsystems_) {
				subsystem->computeState();
			}

			if (controller_ != nullptr)
				controller_->run();
			
			for (SubsystemPtr& subsystem : subsystems_) {
				subsystem->run();
			}

			double elapsed_time = frc::Timer::GetFPGATimestamp() - initial_time > target_loop_time_;
			if (elapsed_time < target_loop_time_) {
				frc::Wait(target_loop_time_ - elapsed_time);
			} else if (elapsed_time > target_loop_time_) {
				message_logger_.startMessage(messageLogger::messageType::warning) ;
				message_logger_ << "Robot loop exceeded target loop time\n";
				message_logger_ << "Loop time: " << elapsed_time << "\n";
				message_logger_.endMessage() ;
			}
		}

		void Robot::RobotInit() {
			//
			// This is a virtual robot.  The actual robot class (i.e. Phoenix) will
			// override this method and create all of the subsystems.  We put an assert
			// here so the program prints information about what needs to be done and
			// then crashes.
			//
			std::cerr << "RobotInit() should be defined in the top level robot class" << std::endl;
			assert(false) ;
		}

		void Robot::Autonomous() {
			message_logger_.startMessage(messageLogger::messageType::info) ;
			message_logger_ << "Starting Autonomous mode" ;
			message_logger_.endMessage() ;

			controller_ = createAutoController() ;
			
			while (IsAutonomous() && IsEnabled())
				robotLoop();

			controller_ = nullptr ;
		}

		void Robot::OperatorControl() {
			message_logger_.startMessage(messageLogger::messageType::info) ;
			message_logger_ << "Starting Teleop mode" ;
			message_logger_.endMessage() ;

			controller_ = createTeleopController() ;

			while (IsOperatorControl() && IsEnabled())
				robotLoop();

			controller_ = nullptr ;
		}

		void Robot::Test() {
			message_logger_.startMessage(messageLogger::messageType::info) ;
			message_logger_ << "Starting Test mode" ;
			message_logger_.endMessage() ;

			controller_ = createTestController() ;

			while (IsTest() && IsEnabled())
				robotLoop();

			controller_ = nullptr ;
		}

		void Robot::Disabled() {
			message_logger_.startMessage(messageLogger::messageType::info) ;
			message_logger_ << "Starting Disables mode" ;
			message_logger_.endMessage() ;

			while (IsDisabled())
				frc::Wait(target_loop_time_) ;
		}

		messageLogger& Robot::getMessageLogger() {
			return message_logger_;
		}
	}
}
