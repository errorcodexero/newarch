#pragma once

#include <memory>
#include <string>

namespace xero {
    namespace base {
        class Action {
        public:
			/// \brief Start the directive; called once per directive when it starts
            virtual void start() = 0 ;

			/// \brief Manage the directive; called each time through the robot loop
            virtual void run() = 0 ;

			/// \brief End the directive; called once per directive when it finished
			// QUESTION: who calls this method ????
            virtual void end() = 0 ;

			/// \brief Cancel the directive
			/// \returns QUESTION: What should this return?
            virtual bool cancel() = 0 ;

			/// \brief Return true if the directive is complete
			/// \returns True if the directive is complete
            virtual bool isDone() = 0 ;

            virtual std::string toString() = 0 ;
        private:
        } ;

        typedef std::shared_ptr<Action> ActionPtr ;
    }
}
