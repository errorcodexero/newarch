#pragma once

#include <vector>
#include "Action.h"

namespace xero {
	namespace base {
		class ActionSequence : public Action {
		public:
			ActionSequence(bool block);
			void start();
			void end();
			void run();
			bool isDone();
			bool cancel();
			void pushAction(std::shared_ptr<Action> action);
		private:
			std::vector<std::shared_ptr<Action>> actionSequence_;
			std::vector<std::shared_ptr<Action>>::iterator index_;
		};
	}
}