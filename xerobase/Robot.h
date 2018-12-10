#pragma once

#include "Subsystem.h"
#include "MessageLogger.h"
#include "SettingsParser.h"
#include <WPILib.h>
#include <memory>
#include <list>
#include <fstream>

/// \file


namespace xero {
	namespace base {

		//
		// Forward reference to the Subsystem class
		//
		class Subsystem ;
		class ControllerBase ;
		class AutoController ;
		class DriveBase ;
		class OISubsystem ;

		/// \brief the base robot class for Error Code Xero robots
		/// This class manages the operation of the robot.  The robot for a specific
		/// year is derived from this class.  The derived class is responsible for creating
		/// the subsystems for the robot and this class is responsible for managing the
		/// operation of the robot.
		/// <br>
		/// This main function here is the robot loop.  The robot loop is run in a fixed
		/// time schedule.  This time schedule defaults to 50 ms but can be changed by the
		/// derived class by calling setRobotLoopTime().  The robot loop performs the following
		/// actions:
		/// <br>
		/// First, the robot calls each of the subsystems to compute their state.  Each subsystem reads
		/// any sensors associated with the subsystem and computes a state that is meaningful
		/// to the robot has a whole.  For instance, for a drivebase, the state might be the total
		/// distance traveled, the x and y position of the robot, and the current direction the robot
		/// is facing.
		/// <br>
		/// Second, the robot calls the controller for the robot.  For each robot mode (autonomous, 
		/// operator, or test), there is a controller.  It is the responsibility of the derived class to
		/// create the controller (see createAutoController(), createTeleopController(), and createTestController).
		/// This controller processes the state of the subsystems and directs subsystems to perform Actions.
		/// <br>
		/// Finally, the robot calls each of the subsytems to execute on any assigned actions.
		///
		class Robot : public frc::SampleRobot {
		public:
			/// \brief create the base robot.
			/// \param looptime the loop time for each robot loop (generally between 0.02 and 0.05)
			Robot(double looptime = 0.050) ;

			/// \brief destroy the robot object
			virtual ~Robot() ;

			/// \brief called to initialize the robot.
			/// This method will be overridden by the derived class that is a
			/// concrete robot class.  This method will assert if called.
			virtual void RobotInit() ;

			/// \brief run the autonomous mode (software in charge)
			virtual void Autonomous() ;

			/// \brief run the teleop mode (drive in charge)
			virtual void OperatorControl() ;

			/// \brief run the test mode (testing in the pits)
			virtual void Test() ;

			/// \brief run the disabled mode (doing nothing)
			virtual void Disabled() ;

			/// \brief Return a reference to the one message logger
			/// \returns a reference to the one message logger
			/// \brief Return reference to the one message logger
			xero::misc::MessageLogger& getMessageLogger() {
				return message_logger_ ;
			}

			/// \brief Return the target loop time for the robot
			/// \returns the target loop time for the robot
			double getTargetLoopTime() {
				return target_loop_time_ ;
			}

			/// \brief Return the time difference between the last robot loop and the current one in seconds
			/// \returns the time difference between the last robot loop and the current one in seconds
			double getDeltaTime()  {
				return delta_time_ ;
			}

			/// \brief Return the current time in seconds
			/// \returns the current time in seconds
			double getTime() {
				return frc::Timer::GetFPGATimestamp() ;
			}

			/// \brief Return a reference to the one settings parser
			/// \return a reference to the one settings parser
			xero::misc::SettingsParser& getSettingsParser() {
				return *parser_ ;
			}

			/// \brief Return the drive base subsystem
			/// \returns the drivebase subsystem
			std::shared_ptr<DriveBase> getDriveBase() {
				return drivebase_subsystem_ ;
			}

			/// \brief return the OIsubsystem
			/// \returns the OI subsystems
			std::shared_ptr<OISubsystem> getOI() {
				return oi_subsystem_ ;
			}			

		protected:

			/// \brief The type of robot loop we are running
			enum class LoopType : int {
				OperatorControl = 0,				///< Operator control robot loop
				Autonomous = 1,						///< Autnomous robot loop
				Test = 2,							///< Test robot loop
				MaxValue = 3						///< Not a loop type, used to get maximum value
			} ;		

			/// \brief add a subsystem to the robot
			/// \param sub the subsystem to add to the robot
			/// \param oi the subsystem for the OI
			/// \param db the subsystem for the drive base
			void setRobotSubsystem(SubsystemPtr sub, std::shared_ptr<OISubsystem> oi, std::shared_ptr<DriveBase> db) {
				robot_subsystem_ = sub ;
				robot_subsystem_->init() ;

				drivebase_subsystem_ = db ;
				oi_subsystem_ = oi ;
			}		
			
			/// \brief return the robot subsystem
			/// \returns the robot subsystem
			SubsystemPtr getRobotSubsystem() {
				return robot_subsystem_ ;
			}
			
			/// \brief this method runs one loop for the robot.
			virtual void robotLoop(LoopType type);

			/// \brief this method reads the parameters file for the robot
			/// \param filename the name of the file to read parameters from
			/// \returns true if the file is read sucessfully
			bool readParamsFile(const std::string &filename) ;

			//
			// These methods are overridden by the actual robot class because
			// the controllers are specific to a given robot
			//

			/// \brief create the autonomous controller.
			/// This method will be defined by a concrete derived robot object
			/// \returns the auto controller for the robot
			virtual std::shared_ptr<ControllerBase> createAutoController() = 0 ;

			/// \brief create the teleop controller.
			/// This method will be defined by a concrete derived robot object
			/// \returns the teleop controller for the robot
			virtual std::shared_ptr<ControllerBase> createTeleopController() = 0 ;
			
			/// \brief create the test controller.
			/// This method will be defined by a concrete derived robot object
			/// \returns the test controller for the robot
			virtual std::shared_ptr<ControllerBase> createTestController() = 0 ;

			/// \brief setup a message logger to send output to a file
			/// \param file the name of the file for the output
			void setupRobotOutputFile(const std::string &file) ;

			/// \brief initialize the robot hardware
			virtual void RobotHardwareInit() ;

			/// \brief return the auto mode selection
			virtual int getAutoModelSelection() ;

		private:
			void logAutoModeState() ;
			void displayAutoModeState() ;
			void updateAutoMode() ;

		private:
			// The time per robot loop in seconds
			double target_loop_time_;

			// The controller that provides control during the
			// robot loop
			std::shared_ptr<ControllerBase> controller_;

			// Auto mode controller, created at the start and stored
			std::shared_ptr<AutoController> auto_controller_;			

			// The list of subsystem that belong to the robot
			SubsystemPtr robot_subsystem_ ;
			std::shared_ptr<DriveBase> drivebase_subsystem_ ;
			std::shared_ptr<OISubsystem> oi_subsystem_ ;

			// Message logger instance
			xero::misc::MessageLogger message_logger_;

			// The time the last time through the robot loop
			double last_time_ ;

			// The time between the last robot lo
			double delta_time_ ;

			// The settings parser
			xero::misc::SettingsParser *parser_ ;

			// The name of an output file for the robot output
			std::string output_file_name_ ;

			// The stream for the robot output
			std::ofstream *output_stream_ ;

			// The selected auto mode
			int automode_ ;

			// The game specific message data
			std::string gamedata_ ;

			std::vector<double> sleep_time_ ;
			std::vector<size_t> iterations_ ;
		} ;
	}
}
