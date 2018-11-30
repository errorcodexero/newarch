#pragma once

#include "Action.h"
#include <list>

namespace xero {
    namespace base {
		/// \brief A class capable of doing work
        class ParallelAction : public Action {
        public:
            /// \brief add a parallel action
            void addAction(std::shared_ptr<Action> action) {
                action_list_.push_back(action) ;
            }

			/// \brief Start the directive; called once per directive when it starts
            virtual void start() ;

			/// \brief Manage the directive; called each time through the robot loop
            virtual void run() ;

			/// \brief Cancel the directive
            virtual void cancel() ;

			/// \brief Return true if the directive is complete
			/// \returns True if the directive is complete
            virtual bool isDone() ;

            /// \brief return a human readable string representing the action
            virtual std::string toString() ;

        private:
            std::list<std::shared_ptr<Action>> action_list_ ;

        };

        typedef std::shared_ptr<Action> ActionPtr ;
    }
}
