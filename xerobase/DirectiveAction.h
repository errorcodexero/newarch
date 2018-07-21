#pragma once
#include "Action.h"
#include "Subsystem.h"

namespace xero {
	namespace base {
		class DirectiveAction : public Action {
		public:
			DirectiveAction(std::shared_ptr<Subsystem> subsystem, std::shared_ptr<Action> directive, bool block);
			void start();
			void end();
			void run();
			bool isDone();
			bool cancel();
		private:
			std::shared_ptr<Subsystem> subsystem_;
			std::shared_ptr<Action> directive_;
		};
	}
}