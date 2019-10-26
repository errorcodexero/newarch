#pragma once

#include <actions/Action.h>

namespace xero {
    namespace ranseur {
        class TubCollector ;

        class TubCollectorAction : public xero::base::Action {
        public:
            TubCollectorAction(TubCollector &tubcol) : tubcollector_(tubcol) {                
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
