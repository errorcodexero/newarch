#pragma once
#include "Action.h"
#include "Subsystem.h"

namespace xero {
	namespace base {
		class DispatchAction : public Action {
		public:
			DispatchAction(SubsystemPtr subsystem, ActionPtr action, bool block);
			void start();
			void end();
			void run();
			bool isDone();
			bool cancel();
			std::string toString();
		private:
			std::shared_ptr<Subsystem> subsystem_;
			std::shared_ptr<Action> action_;
			bool block_;
			bool denied_;
		};
	}
}