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
			std::string toString();
		private:
			double startTime_;
			double delayTime_;
			bool isDone_;
		};
	}
}