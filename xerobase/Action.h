#pragma once
#include <memory>
#include <vector>
#include <Timer.h>
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
            virtual void cancel() = 0 ;

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
