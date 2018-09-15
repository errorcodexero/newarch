#pragma once

#include <Robot.h>
#include <GPG3MotorController.h>


namespace xero {
	namespace gopigo {

		/// \brief the concrete class that represents the robot Phoenix
		class GoPiGo3Xero : public xero::base::Robot {

			/// \brief called to initialize the robot, basically creating the subsystems
			virtual void RobotInit() ;

			/// \brief called to create the autonomous controller
			/// \returns a robot controller
			virtual std::shared_ptr<xero::base::ControllerBase> createAutoController() ;
			
			/// \brief called to create the teleop controller
			/// \returns a robot controller
			virtual std::shared_ptr<xero::base::ControllerBase> createTeleopController() ;

			/// \brief called to create the test controller
			/// \returns a robot controller
			virtual std::shared_ptr<xero::base::ControllerBase> createTestController() ;

		private:
			void initializeMessageLogger();
		} ;
	}
}
