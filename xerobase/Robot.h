#pragma once

#include <WPILib.h>
#include <memory>
#include <list>

namespace xero {
	namespace base {

		//
		// Forward reference to the Subsystem class
		//
		class Subsystem ;
		class ControllerBase ;

		/// \brief the base robot class for Error Code Xero robots
		class Robot : public frc::SampleRobot {
		public:
			/// \brief create the base robot.
			Robot() ;

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

		protected:
			//
			// Useful types, moved these into the class so we don't
			// pollute the global name space
			//
			// COMMENT - moved these into the class so they do not pollute
			//           the global namespace
			//
			typedef std::shared_ptr<Subsystem> SubsystemPtr;
			typedef std::list<SubsystemPtr> SubsystemList;

			
			/// \brief this method runs one loop for the robot.
			virtual void robotLoop();

			//
			// These methods are overridden by the actual robot class because
			// the controllers are specific to a given robot
			//

			/// \brief create the autonomous controller.
			/// This method will be defined by a concrete derived robot object
			virtual std::shared_ptr<ControllerBase> createAutoController() = 0 ;

			/// \brief create the teleop controller.
			/// This method will be defined by a concrete derived robot object
			virtual std::shared_ptr<ControllerBase> createTeleopController() = 0 ;
			
			/// \brief create the test controller.
			/// This method will be defined by a concrete derived robot object
			virtual std::shared_ptr<ControllerBase> createTestController() = 0 ;

			/// \brief add a subsystem to the robot
			/// \param sub the subsystem to add to the robot
			void addSubsystem(SubsystemPtr sub) {
				subsystems_.push_back(sub) ;
			}

		private:
			// The time per robot loop in seconds
			double target_loop_time_;


			// The controller that provides control during the
			// robot loop
			std::shared_ptr<ControllerBase> controller_;

			// The list of subsystem that belong to the robot
			SubsystemList subsystems_;
		} ;
	}
}
