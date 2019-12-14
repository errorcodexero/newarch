#pragma once

#include <actions/Action.h>

namespace xero {
    namespace phoenix {
        class Collector ;

        class CollectorAction : public xero::base::Action {
        public:
            CollectorAction(Collector &col) : collector_(col) {                
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
