#pragma once

#include "ControllerBase.h"

namespace xero {
	namespace base {
		class TestController : public ControllerBase {
		public:
			TestController(Robot &robot) : ControllerBase(robot) {
			}

			virtual ~TestController() {
			}

			virtual void run() {
			}

		private:
		} ;
	}
}