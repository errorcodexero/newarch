#pragma once

#include <Action.h>

namespace xero {
    namespace phoenix {
        class LiftingCollector ;

        class LiftingCollectorAction : public xero::base::Action {
        public:
            LiftingCollectorAction(LiftingCollector &col) : collector_(col) {                
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
