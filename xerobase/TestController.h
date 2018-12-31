#pragma once

/// \file

#include "ControllerBase.h"

namespace xero {
	namespace base {
        /// \brief This class is the base class for the test controller
        /// The test controller is currently not used in the system but any robot
        /// can derive a class from this class and do what it wants with the test
        /// mode.
		class TestController : public ControllerBase {
		public:
            /// \brief create the test controller
            /// \param robot the robot object this controller is controlling
			TestController(Robot &robot) : ControllerBase(robot) {
			}

            /// \brief destroy the test controller object
			virtual ~TestController() {
			}

            /// \brief run the test controller
			virtual void run() {
			}

		private:
		} ;
	}
}