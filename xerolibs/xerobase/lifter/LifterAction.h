#pragma once

#include <actions/Action.h>

namespace xero {
    namespace base {
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