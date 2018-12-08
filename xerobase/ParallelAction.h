#pragma once

#include "Action.h"
#include <list>

/// \file


namespace xero {
    namespace base {
		/// \brief A class capable of doing work
        class ParallelAction : public Action {
        public:
            /// \brief add a parallel action
            /// \param action an action to be added to the parallel action
            void addAction(std::shared_ptr<Action> action) {
                action_list_.push_back(action) ;
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

			/// \brief Return true if the action is complete.
			/// This method returns true only when all of the parallel actions are complete.
			/// \returns True if the action is complete
            virtual bool isDone() ;

			/// \brief return a human readable string representing the action
			/// \returns a human readable strinct
            virtual std::string toString() ;

        private:
            std::list<std::shared_ptr<Action>> action_list_ ;

        };

        typedef std::shared_ptr<Action> ActionPtr ;
    }
}
