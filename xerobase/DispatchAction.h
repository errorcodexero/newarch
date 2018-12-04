#pragma once
#include "Action.h"
#include "Subsystem.h"

namespace xero {
	namespace base {
		/// \brief this is a an action that wraps a subsystem and an action to assign to a subsystem
		class DispatchAction : public Action {
		public:
			/// \brief create a new wrapper to wrap a subsystem and an assocaited action
			/// \param subsystem the subsystem to assign an action to
			/// \param action the action to assign to the subsystem
			/// \param block if true, wait for the subsystem to complete the action before returning isDone() true
			DispatchAction(SubsystemPtr subsystem, ActionPtr action, bool block = true);

			/// \brief start this wrapper action which just assigns the stored action to the subsystem
			void start();

			/// \brief if this operation is blocking checks the subsystem to see if the assigned action is complete
			void run();

			/// \brief if non blocking, returns true immeidately, otherwise returns true when the subsystem completes the action
			/// \returns true when the action is complete
			bool isDone();

			/// \brief if the operation is blocking, cancels the operation on the subsystem
			void cancel();

			/// \brief returns a human readable string representing this wrapped action
			/// \returns a human readable string for this action
			std::string toString();

			/// \brief returns true if the subsystem did not accept the action
			/// \returns true if the action was not accepted by the subsystem
			bool isDenied() { return denied_ ;}
			
		private:
			std::shared_ptr<Subsystem> subsystem_;
			std::shared_ptr<Action> action_;
			bool block_;
			bool denied_;
		};
	}
}