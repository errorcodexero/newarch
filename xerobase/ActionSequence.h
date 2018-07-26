#pragma once

#include <vector>
#include "Action.h"
#include "DispatchAction.h"

namespace xero {
	namespace base {
		class ActionSequence : public Action {
		public:
			ActionSequence();
			void start();
			void end();
			void run();
			bool isDone();
			bool cancel();
			void pushAction(ActionPtr action);
			void pushSubActionPair(SubsystemPtr subsystem, ActionPtr action) {
				pushAction(std::make_shared<DispatchAction>(subsystem, action));
			}
		private:
			bool isDone_;
			std::vector<ActionPtr> actionSequence_;
			std::vector<ActionPtr>::iterator index_;
			bool performActionForIsDoneStatus(ActionPtr action);
		};
	}
}