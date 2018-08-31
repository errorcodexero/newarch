#pragma once

#include "CollectorAction.h" 

namespace xero {
    namespace phoenix {
        class Collector;
        class CollectorEjectCubeAction : public xero::phoenix::CollectorAction {
            public:
                CollectorEjectCubeAction(Collector &collector);
                virtual ~CollectorEjectCubeAction(); 
            protected:
               
            private:
               
        };
    }
}