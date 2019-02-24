#pragma once
#include <frc/Timer.h>
#include <memory>
#include <vector>
#include <string>
#include <memory>

/// \file


namespace xero {
    namespace base {
        /// \brief A class capable of doing work.
        /// An action is the basis of all activity on the robot both in operator mode and autonomous mode.  
        /// There are some actions that are independent of subsystems are part of the control of the robot.  However
        /// the vast majority of actions are used to ask a robot subsystem to perform some function.  For instance,
        /// one of the actions for the TankDrive subsystem is to drive straight a fixed distance.
        /// \sa ParallelAction
        /// \sa DelayAction
        /// \sa ActionSequence
        class Action {
        public:
            /// \brief Start the action; called once per action when it starts
            virtual void start() = 0 ;

            /// \brief Manage the action; called each time through the robot loop
            virtual void run() = 0 ;

            /// \brief Cancel the action
            /// A cancel operation may take many cycles to complete and this action should
            /// not return isDone() equal true until the cancel has completed.  A cancel should
            /// restore the robot to a reasonable state.
            virtual void cancel() = 0 ;

            /// \brief abort the current action immediately.  
            /// The action should return isDone() equal true immedately after
            /// executing this method.  The default implementation of this calls
            /// cancelAction as this is how cancelAction works for most actions.  However
            /// if an action does not complete immediately after cancelAction() is
            /// called, abortAction() should be defined to end all actions immediately.
            virtual void abort() {
                cancel() ;
            }            

            /// \brief Return true if the action is complete
            /// \returns True if the action is complete
            virtual bool isDone() = 0 ;

            /// \brief return a human readable string representing the action
            /// \returns a human readable string representing the action
            virtual std::string toString() = 0 ;
        private:
        };

        /// \brief a shared pointer to an action
        typedef std::shared_ptr<Action> ActionPtr ;
    }
}
