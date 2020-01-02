#pragma once

#include "phlifter/Lifter.h"
#include <actions/Action.h>
#include <Robot.h>

namespace xero {
    namespace phoenix {
        class Lifter ;

        class LifterAction : public xero::base::GenericAction {
        public:
            LifterAction(Lifter &lifter) : GenericAction(lifter.getRobot().getMessageLogger()), lifter_(lifter) {                
            }

            Lifter &getLifter() {
                return lifter_ ;
            }

        private:
            Lifter &lifter_ ;
        } ;
    }
}