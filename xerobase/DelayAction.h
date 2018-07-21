#pragma once
#include "Action.h"
#include <Timer.h>

namespace xero {
	namespace base {
		class DelayAction : public Action {
		public:
			DelayAction();
			DelayAction(double delayTime_);
			void start();
			void end();
			void run();
			bool isDone();
			bool cancel();
			void setDelayTime(double delayTime);
		private:
			frc::Timer timer_;
			double delayTime_;
		};
	}
}