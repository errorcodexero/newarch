#pragma once

#include "RanseurAutoModeBase.h"
#include <actions/ParallelAction.h>

namespace xero
{
    namespace ranseur 
    {
        class AutoModeXeroTest : public RanseurAutoModeBase
        {
        public:
            AutoModeXeroTest(xero::base::Robot &robot) ;
            virtual ~AutoModeXeroTest() ;

        private:
        } ;

    }
}


