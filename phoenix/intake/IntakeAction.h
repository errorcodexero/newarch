#pragma once

#include "Action.h" 

namespace xero {
    namespace phoenix {
        class Intake ;

        class IntakeAction : public xero::base::Action {
        public:
            IntakeAction(Intake &intake) ;
            virtual ~IntakeAction() ;
        protected:
            Intake & getIntake() {
                return intake_ ;
            }
        private:
            Intake &intake_ ;
        };
    }
}