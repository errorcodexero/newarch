#pragma once
#include "MessageLogger.h"
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
        /// \sa SequenceAction
        class Action {
        public:
            Action(xero::misc::MessageLogger &logger);
            virtual ~Action();

            /// \brief Start the action; called once per action when it starts
            virtual void start();

            /// \brief Manage the action; called each time through the robot loop
            virtual void run();

            /// \brief Cancel the action
            /// A cancel operation should complete immediately.
            virtual void cancel();

            /// \brief Return true if the action is complete
            /// \returns True if the action is complete
            virtual bool isDone() {
                return is_done_;
            }

            /// \brief return a human readable string representing the action
            /// \returns a human readable string representing the action
            virtual std::string toString() = 0 ;

            xero::misc::MessageLogger &getMessageLogger() {
                return logger_;
            }

        protected:
            void setDone();

            void setNotDone() {
                is_done_ = false;
            }

        private:
            // Set to true when this action is done
            bool is_done_;
            xero::misc::MessageLogger &logger_;

            uint32_t action_id_;

            static uint32_t global_action_id_;
        };

        /// \brief a shared pointer to an action
        typedef std::shared_ptr<Action> ActionPtr ;


        /// A \c CompositeAction is an action composed of one or more child actions.
        class CompositeAction: public Action {
        public:
            CompositeAction(xero::misc::MessageLogger &logger) : Action(logger)
            {
            }

            virtual ~CompositeAction()
            {                
            }

            virtual void start() {
                Action::start();
            }

            virtual void run() {
                Action::run();
            }

            virtual void cancel() {
                Action::cancel();
            }

            /// \returns All of this action's children.
            virtual std::vector<ActionPtr> getChildren() = 0;
        };

        /// An action that can be applied to any subsystem.
        class GenericAction: public Action 
        {
        public:
            GenericAction(xero::misc::MessageLogger &logger) : Action(logger)
            {                
            }

            virtual ~GenericAction()
            {                
            }

            virtual void start() {
                Action::start();
            }

            virtual void run() {
                Action::run();
            }

            virtual void cancel() {
                Action::cancel();
            }

        };
    }
}
