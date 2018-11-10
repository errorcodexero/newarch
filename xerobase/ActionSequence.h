#pragma once
#include "Action.h"
#include "DispatchAction.h"
#include <MessageLogger.h>
#include <memory>
#include <vector>
#include <string>

namespace xero {
	namespace base {
		/// \brief this class stores and executes a sequence of actions.
		/// Actions are added to this action sequence object.  When run() is called
		/// this sequence of action is ececuted in sequence until they are all complete.
		/// When an action in the sequence is complete (e.g. isDone() returns true), 
		/// the sequence moves to the next action.  This continues until all actions
		/// in the sequence are complete.  Once all actions in the sequence are 
		/// complete, this object will return isDone() true.
		class ActionSequence : public Action {
		public:
			/// \brief create an empty action sequence 
			ActionSequence(xero::misc::MessageLogger &logger, const std::string &name);

			/// \brief start this sequence of actions
			void start();

			/// \brief called each time through the robot loop to execute the next block of work for the sequence.
			/// This method calls run on the currently active action in the action sequence.
			void run();

			/// \brief returns false until all actions in the sequence are complete, or until cancel is called.
			/// Once all actions in the sequence are complete, or if cancel is called, then this returns true.
			bool isDone();

			/// \brief cancel the action sequence.
			/// This method calls cancel on the currently active action, then sets the internal state of this 
			/// sequence such that it appears this sequence is complete.  None of the remaining actions will be
			/// processed.
			void cancel();

			/// \brief push a new action to the end of the action sequence
			/// \param action the action to add to the sequence
			void pushAction(ActionPtr action);

			/// \brief push a new subsystem action pair to the end of the sequence
			/// The action sequence cannot execute a subsystem action directly.  Rather a subsystem action
			/// must be assigned to the required subsystem.  This method puses a wrapper action into the
			/// action sequence that stores both the action and the associated subsystem.  The wrapper can
			/// assign the action and continue, or the wrapper can assign the action and wait for it to be
			/// complete.  The block parameter controls this beharior.  If block is true, this wrapper will
			/// wait for the subsystem action to complete.
			/// \param subsystem the subsystem that will receive the action
			/// \param action the action that is assigned to the subsystem
			/// \param block if true the sequence waits for the subsystem to complete the action
			void pushSubActionPair(SubsystemPtr subsystem, ActionPtr action, bool block = true);

			/// \brief create a human readable string representing this action sequence
			/// \returns a human readable string representing this action sequence
			std::string toString();

			/// \brief return the name of the action sequence
			/// \returns the name of the action sequence
			const std::string &getName() const {
				return name_ ;
			}

			/// \brief clear the list of actions
			void clear() {
				actionSequence_.clear() ;
			}

			/// \brief return the number of actions in the sequence
			size_t size() {
				return actionSequence_.size() ;
			}			

		private:
			void startNextAction();

		private:
			// state of the sequence, true when all contained actions are complete or cancel is called
			bool isDone_;

			// the seqeuence of actions
			std::vector<ActionPtr> actionSequence_;

			// the index of the current action
			int index_;

			// The message group for the message logger
			int group_ ;

			// The message logger
			xero::misc::MessageLogger &logger_ ;

			//names the overall objective of the actions in the sequence
			std::string name_;
		};

		typedef std::shared_ptr<ActionSequence> ActionSequencePtr ;
	}
}
