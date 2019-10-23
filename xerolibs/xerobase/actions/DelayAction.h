#pragma once
#include "actions/Action.h"
#include <frc/Timer.h>

/// \file


namespace xero {
    namespace base {
        /// \brief an action that delays for a fixed amount of time.  This step
        /// is guaranteed to delay at least for the delay given but may delay
        /// more based on the specific scenario.
        class DelayAction : public GenericAction {
        public:
            /// \brief create a delay object with the delay given
            /// \param delayTime the delay in seconds
            DelayAction(double delayTime);

            /// \brief start the delay action, starting the delay period
            void start();

            /// \brief run the action, does nothing for the delay
            void run();

            /// \brief returns true when the desired delay has occured
            /// \returns true when the delay has elapsed
            bool isDone();

            /// \brief cancel the delay, isDone will return true immediately
            void cancel();

            /// \brief return a human readable string represnting this step
            /// \returns a human readable string for this action
            std::string toString();
            
        private:
            double startTime_;
            double delayTime_;
            bool isDone_;
        };
    }
}