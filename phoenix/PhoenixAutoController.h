#pragma once

#include "AutoController.h"
#include "Action.h"

namespace xero {
	namespace base {
		/// \brief This is the base class for any autonomous mode controller
		/// This class contains the code to execute an automode program and
		/// the derived classs generally creates the auto program for a given
		/// robot, automode selector, and set of field conditions.
		class PhoenixAutoController : public AutoController
		{
			public:
			PhoenixAutoController(ActionPtr action, Robot &robot):AutoController(robot){
				setAction(action);
			}
		};

	}
}
