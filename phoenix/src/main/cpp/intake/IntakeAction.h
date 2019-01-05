#pragma once

#include "Action.h" 

namespace xero {
    namespace phoenix {
        class Intake ;

        /// \brief This is the base class for all Intake actions
        class IntakeAction : public xero::base::Action {
        public:
            /// \brief create a new IntakeAction
            /// \param intake the subsystem this action will interact with
            IntakeAction(Intake &intake) ;

            /// \brief destroy an intake action
            virtual ~IntakeAction() ;

        protected:
            /// \brief return a reference to the intak subsystem
            Intake & getIntake() {
                return intake_ ;
            }

        private:
            // The intake subsystem
            Intake &intake_ ;
        };
    }
}

