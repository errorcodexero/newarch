#pragma once

#include "BunnySubsystem.h" 
#include "sorterin/SorterIn.h"
#include "sorterout/SorterOut.h"

namespace xero {
    namespace bunny2018 {
		
        class Sorter : public xero::base::Subsystem {

        public:
            Sorter(xero::base::Robot & robot);

            virtual ~Sorter(); 

        private:
            std::shared_ptr<SorterIn> sorter_in_;

            std::shared_ptr<SorterOut> _;


        };
    }
}