#pragma once

#include "RanseurAutoModeBase.h"
#include <actions/ParallelAction.h>

namespace xero
{
    namespace ranseur 
    {
        class AutoModeXero : public RanseurAutoModeBase
        {
        public:
            AutoModeXero(xero::base::Robot &robot) ;
            virtual ~AutoModeXero() ;

        private:
        } ;

    }
}


