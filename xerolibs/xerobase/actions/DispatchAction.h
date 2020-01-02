#pragma once
#include "actions/Action.h"
#include "Subsystem.h"

/// \file


namespace xero {
    namespace base {
        /// \brief this is a an action that wraps a subsystem and an action to assign to a subsystem
        class DispatchAction : public Action {
        public:
            /// \brief create a new wrapper to wrap a subsystem and an associated action
            /// \param subsystem the subsystem to assign an action to
            /// \param action the action to assign to the subsystem
            /// \param block if true, wait for the subsystem to complete the action before returning isDone() true
            DispatchAction(SubsystemPtr subsystem, ActionPtr action, bool block = true);

            /// \brief start this wrapper action which just assigns the stored action to the subsystem
            void start();

            /// \brief if this operation is blocking checks the subsystem to see if the assigned action is complete
            void run();

            /// \brief if the operation is blocking, cancels the operation on the subsystem
            void cancel();

            /// \brief returns a human readable string representing this wrapped action
            /// \returns a human readable string for this action
            std::string toString();

            /// \returns A pointer to the subsystem.
            SubsystemPtr getSubsystem() { return subsystem_; }

            /// \returns A pointer to the action.
            ActionPtr getAction() { return action_; }
        private:
            //
            // The subsystem to assign the action to
            //
            std::shared_ptr<Subsystem> subsystem_;

            //
            // The action to assign to a subsystem, must be compatible with the
            // subsystem.
            //
            std::shared_ptr<Action> action_;

            //
            // This is set true in the constructor when you want
            // the subsystem action to complete before this action
            // completes.
            //
            bool block_;
        };
    }
}