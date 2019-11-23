#pragma once

#include "RanseurAutoModeBase.h"
#include <actions/ParallelAction.h>

namespace xero
{
    namespace ranseur 
    {
        class AutoModeTest : public RanseurAutoModeBase
        {
        public:
            AutoModeTest(xero::base::Robot &robot) ;
            virtual ~AutoModeTest() ;

        private:
        } ;

    }
}


