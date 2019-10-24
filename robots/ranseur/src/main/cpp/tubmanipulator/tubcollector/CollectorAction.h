#pragma once

#include <Action.h>

namespace xero {
    namespace ranseur {
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
