#include "Robot.h"
#include "Subsystem.h"
#include "ControllerBase.h"
#include "basegroups.h"
#include <MessageDestStream.h>
#include <DriverStation.h>
#include <iostream>
#include <cassert>

using namespace xero::misc ;

namespace xero {
	namespace base {
		
		Robot::Robot() {
			//
			// Set the robot loop time to 50 ms
			//
			target_loop_time_ = 0.05 ;
			last_time_ = frc::Timer::GetFPGATimestamp() ;

			parser_ = new SettingsParser(message_logger_, MSG_GROUP_PARSER) ;
			output_stream_ = nullptr ;
		}

		Robot::~Robot() {
			delete parser_ ;
			message_logger_.clear() ;
			if (output_stream_ != nullptr)
				delete output_stream_ ;
		}

		void Robot::setupRobotOutputFile(const std::string &file) {
			if (file.length() > 0) {
				output_stream_ = new std::ofstream(file) ;
				if (output_stream_->fail() || output_stream_->bad()) {
					delete output_stream_ ;
					output_stream_ = nullptr ;
				}
				else {
					auto dest = std::make_shared<MessageDestStream>(*output_stream_) ;
					message_logger_.addDestination(dest) ;
				}
			}
		}		

		bool Robot::readParamsFile(const std::string &filename) {
			return parser_->readFile(filename) ;
		}
		
		void Robot::robotLoop() {
			double initial_time = frc::Timer::GetFPGATimestamp();

			delta_time_ = initial_time - last_time_ ;

			robot_subsystem_->computeState() ;

			if (controller_ != nullptr)
				controller_->run();

			robot_subsystem_->run() ;			

			double elapsed_time = frc::Timer::GetFPGATimestamp() - initial_time > target_loop_time_;
			if (elapsed_time < target_loop_time_) {
				frc::Wait(target_loop_time_ - elapsed_time);
			} else if (elapsed_time > target_loop_time_) {
				message_logger_.startMessage(MessageLogger::MessageType::warning) ;
				message_logger_ << "Robot loop exceeded target loop time\n";
				message_logger_ << "Loop time: " << elapsed_time << "\n";
				message_logger_.endMessage() ;
			}

			last_time_ = initial_time ;
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

		void Robot::logAutoModeState() {
			std::string value ;
			frc::DriverStation &ds = frc::DriverStation::GetInstance() ;
			
			message_logger_.startMessage(MessageLogger::MessageType::info) ;
			message_logger_ << "Entering Autonomous mode: " ;
			message_logger_.endMessage() ;

			value = "undefined" ;
			if (ds.GetAlliance() == frc::kRed)
				value = "red" ;
			else if (ds.GetAlliance() == frc::kBlue)
				value = "blue" ;

			message_logger_.startMessage(MessageLogger::MessageType::info) ;
			message_logger_ << "    Alliance: " << value ;
			message_logger_.endMessage() ;

			value = std::to_string(ds.GetLocation()) ;
			message_logger_.startMessage(MessageLogger::MessageType::info) ;
			message_logger_ << "    Location: " << value ;
			message_logger_.endMessage() ;				

			value = ds.GetGameSpecificMessage() ;
			message_logger_.startMessage(MessageLogger::MessageType::info) ;
			message_logger_ << "    GameData: " << value ;
			message_logger_.endMessage() ;		

			value = ds.GetEventName() ;
			message_logger_.startMessage(MessageLogger::MessageType::info) ;
			message_logger_ << "    EventName: " << value ;
			message_logger_.endMessage() ;

			value = "invalid" ;
			frc::MatchType mt = ds.GetMatchType() ;
			switch(mt) {
			case frc::kPractice:
				value = "practice" ;
				break ;
			case frc::kNone:
				value = "none" ;
				break ;
			case frc::kQualification:
				value = "qualification" ;
				break ;
			case frc::kElimination:
				value = "elimination" ;
				break ;												
			}

			message_logger_.startMessage(MessageLogger::MessageType::info) ;
			message_logger_ << "    MatchType: " << value ;
			message_logger_.endMessage() ;

			value = std::to_string(ds.GetMatchNumber()) ;
			message_logger_.startMessage(MessageLogger::MessageType::info) ;
			message_logger_ << "    MatchNumber: " << value ;
			message_logger_.endMessage() ;	

			value = std::to_string(ds.GetReplayNumber()) ;
			message_logger_.startMessage(MessageLogger::MessageType::info) ;
			message_logger_ << "    ReplayNumber: " << value ;
			message_logger_.endMessage() ;						

			value = "No" ;
			if (ds.IsFMSAttached())
				value = "Yes" ;

			message_logger_.startMessage(MessageLogger::MessageType::info) ;
			message_logger_ << "    FMS Attached: " << value ;
			message_logger_.endMessage() ;				
		}

		void Robot::Autonomous() {
			logAutoModeState() ;

			controller_ = createAutoController() ;
			
			while (IsAutonomous() && IsEnabled())
				robotLoop();

			controller_ = nullptr ;

			message_logger_.startMessage(MessageLogger::MessageType::info) ;
			message_logger_ << "Leaving Autonomous mode" ;
			message_logger_.endMessage() ;
		}

		void Robot::OperatorControl() {
			message_logger_.startMessage(MessageLogger::MessageType::info) ;
			message_logger_ << "Starting Teleop mode" ;
			message_logger_.endMessage() ;

			controller_ = createTeleopController() ;

			while (IsOperatorControl() && IsEnabled())
				robotLoop();

			controller_ = nullptr ;

			message_logger_.startMessage(MessageLogger::MessageType::info) ;
			message_logger_ << "Leaving Teleop mode" ;
			message_logger_.endMessage() ;			
		}

		void Robot::Test() {
			message_logger_.startMessage(MessageLogger::MessageType::info) ;
			message_logger_ << "Starting Test mode" ;
			message_logger_.endMessage() ;

			controller_ = createTestController() ;

			while (IsTest() && IsEnabled())
				robotLoop();

			controller_ = nullptr ;

			message_logger_.startMessage(MessageLogger::MessageType::info) ;
			message_logger_ << "Leaving Test mode" ;
			message_logger_.endMessage() ;			
		}

		void Robot::Disabled() {
			message_logger_.startMessage(MessageLogger::MessageType::info) ;
			message_logger_ << "Robot Disabled" ;
			message_logger_.endMessage() ;

			while (IsDisabled()) {
				if (oi_subsystem_ != nullptr)
					oi_subsystem_->computeState() ;
					
				frc::Wait(target_loop_time_) ;
			}
		}
	}
}
