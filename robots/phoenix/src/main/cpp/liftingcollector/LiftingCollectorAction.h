#pragma once

#include "liftingcollector/LiftingCollector.h
#include <actions/Action.h>
#include <Robot.h>

namespace xero {
    namespace phoenix {
        class LiftingCollector ;

        class LiftingCollectorAction : public xero::base::GenericAction {
        public:
            LiftingCollectorAction(LiftingCollector &col) : GenericAction(col.getRobot().getMessageLogger()), collector_(col) {                
            }

            virtual ~LiftingCollectorAction() {                
            }

            LiftingCollector &getLiftingCollector() {
                return collector_ ;
            }

        private:
            LiftingCollector &collector_ ;
        } ;
    }
}
