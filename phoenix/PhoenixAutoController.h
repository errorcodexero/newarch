#pragma once

#include "AutoController.h"
#include "ActionSequence.h"

namespace xero {
	namespace phoenix {
		/// \brief This is the base class for any autonomous mode controller
		/// This class contains the code to execute an automode program and
		/// the derived classs generally creates the auto program for a given
		/// robot, automode selector, and set of field conditions.
		class PhoenixAutoController : public xero::base::AutoController
		{
		public:
			PhoenixAutoController(xero::base::ActionSequencePtr action, xero::base::Robot &robot):AutoController(robot) {
				setAction(action);
			}
		};
	}
}
