#pragma once

#include <Robot.h>

namespace xero {
	namespace phoenix {

		/// \brief the concrete class that represents the robot Phoenix
		class Phoenix : public xero::base::Robot {

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
		} ;
	}
}
