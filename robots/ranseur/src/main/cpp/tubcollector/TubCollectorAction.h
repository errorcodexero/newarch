#pragma once

#include "tubcollector/TubCollector.h"
#include <actions/Action.h>
#include <Robot.h>

namespace xero {
    namespace ranseur {
        class TubCollector ;

        class TubCollectorAction : public xero::base::GenericAction {
        public:
            TubCollectorAction(TubCollector &tubcol) : GenericAction(tubcol.getRobot().getMessageLogger()), tubcollector_(tubcol) {                
            }

            virtual ~TubCollectorAction() {                
            }

            TubCollector &getTubCollector() {
                return tubcollector_ ;
            }

        private:
            TubCollector &tubcollector_ ;
        } ;
    }
}
