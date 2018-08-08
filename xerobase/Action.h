#pragma once
#include <memory>
#include <vector>
#include <Timer.h>
#include <string>
#include <memory>

namespace xero {
    namespace base {
		/// \brief A class capable of doing work
        class Action {
        public:
			/// \brief Start the directive; called once per directive when it starts
            virtual void start() = 0 ;

			/// \brief Manage the directive; called each time through the robot loop
            virtual void run() = 0 ;

			/// \brief Cancel the directive
            virtual void cancel() = 0 ;

			/// \brief Return true if the directive is complete
			/// \returns True if the directive is complete
            virtual bool isDone() = 0 ;

            /// \brief return a human readable string representing the action
            virtual std::string toString() = 0 ;
        private:
        };

        typedef std::shared_ptr<Action> ActionPtr ;
    }
}
