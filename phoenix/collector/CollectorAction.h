#pragma once

#include "Action.h" 

namespace xero {
    namespace phoenix {
        class Collector;
        class CollectorAction : public xero::base::Action {
            public:
                CollectorAction(Collector &Collector);
                virtual ~CollectorAction(); 
            protected:
                Collector & getCollector() {
                    return Collector_ ;
                }
            private:
                Collector &Collector_ ;
        };
    }
}