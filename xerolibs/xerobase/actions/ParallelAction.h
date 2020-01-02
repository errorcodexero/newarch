#pragma once

#include "actions/Action.h"
#include "actions/DispatchAction.h"
#include <list>

/// \file


namespace xero {
    namespace base {
        /// \brief A class capable of doing work
        class ParallelAction : public CompositeAction {
        public:
            ParallelAction(xero::misc::MessageLogger &logger) ;
            virtual ~ParallelAction() ;

            /// \brief add a parallel action
            /// \param action an action to be added to the parallel action
            void addAction(std::shared_ptr<Action> action) {
                action_list_.push_back(action) ;
            }

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
            void addSubActionPair(SubsystemPtr subsystem, ActionPtr action, bool block = true) {
                auto p = std::make_shared<DispatchAction>(subsystem, action, block);
                addAction(p);                
            }

            /// \brief Start the action; called once per action when it starts
            /// This method calls the start method on each of the parallel actions to be executed
            virtual void start() ;

            /// \brief Manage the action; called each time through the robot loop
            /// This method calls the run method on each of the parallel actions to be executes
            virtual void run() ;

            /// \brief Cancel the action
            /// This method calls the cancel method on each of the parallel actions to be executes
            virtual void cancel() ;

            /// \brief return a human readable string representing the action
            /// \returns a human readable strinct
            virtual std::string toString() ;

            virtual std::vector<ActionPtr> getChildren() { return action_list_; }

        private:
            std::vector<std::shared_ptr<Action>> action_list_ ;

        };

        typedef std::shared_ptr<Action> ActionPtr ;
    }
}
