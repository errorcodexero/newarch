#include "Phoenix.h"
#include <Drivebase.h>

using namespace xero::base ;

namespace xero {
	namespace phoenix {
		
		void Phoenix::RobotInit() {
			//
			// This is where the subsystems for the robot get created
			//
			std::shared_ptr<Drivebase> db_p = std::make_shared<Drivebase>(*this) ;

			//
			// QUESTION: how do we add this drivebase to the robot????
			//

			//
			// This is where we would add the other subsystems for the robot
			//
		}

		std::shared_ptr<ControllerBase> Phoenix::createAutoController() {
			//
			// This is where the autonomous controller is created
			//
			return nullptr ;
		}
		
		std::shared_ptr<ControllerBase> Phoenix::createTeleopController() {
			//
			// This is where the teleop controller is created
			//
			return nullptr ;
		}
		 
		std::shared_ptr<ControllerBase> Phoenix::createTestController() {
			//
			// This is where the test controller is created
			//
			return nullptr ;
		}
	}
}

//
// This macro call is defined by the WPI library.  It is the call that connects
// the class xero::phoenix::Phoenix to physical robot
//
START_ROBOT_CLASS(xero::phoenix::Phoenix) ;
