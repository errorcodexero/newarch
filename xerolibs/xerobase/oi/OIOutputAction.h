#pragma once

#include "actions/Action.h"

namespace xero {
    namespace base {
        /// \brief an action to light a light on the OI
        class OIOutputAction : GenericAction {
        public:
            /// \brief create an output action
            /// \param hid the HID device to control
            /// \param output the specific output to control
            /// \param state the state of the output
            OIOutputAction(xero::misc::MessageLogger &logger, int hid, int output, int state) ;

            /// \brief destroy the action
            virtual ~OIOutputAction() ;

            /// \brief Start the action; called once per action when it starts
            virtual void start() ;

            /// \brief Manage the action; called each time through the robot loop
            virtual void run()  ;

            /// \brief Cancel the action
            virtual void cancel() ;

            /// \brief return a human readable string representing the action
            /// \returns a human readable string representing the action
            virtual std::string toString()  ;

        private:
            int hid_ ;
            int output_ ;
            int state_ ;
        } ;
    }
}