#pragma once

#include "Collector.h"
#include <actions/Action.h>
#include <Robot.h>

namespace xero {
    namespace phoenix {
        class Collector ;

        class CollectorAction : public xero::base::GenericAction {
        public:
            CollectorAction(Collector &col) : GenericAction(col.getRobot().getMessageLogger()), collector_(col) {                
            }

            virtual ~CollectorAction() {                
            }

            Collector &getCollector() {
                return collector_ ;
            }

        private:
            Collector &collector_ ;
        } ;
    }
}
