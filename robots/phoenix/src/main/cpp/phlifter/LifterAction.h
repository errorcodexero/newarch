#pragma once

#include <actions/Action.h>

namespace xero {
    namespace phoenix {
        class Lifter ;

        class LifterAction : public xero::base::Action {
        public:
            LifterAction(Lifter &lifter) : lifter_(lifter) {                
            }

            Lifter &getLifter() {
                return lifter_ ;
            }

        private:
            Lifter &lifter_ ;
        } ;
    }
}