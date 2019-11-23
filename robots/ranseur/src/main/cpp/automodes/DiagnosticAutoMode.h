#pragma once

#include "RanseurAutoModeBase.h"
#include <actions/ParallelAction.h>

namespace xero
{
    namespace ranseur 
    {
        class DiagnosticAutoMode : public RanseurAutoModeBase
        {
        public:
            DiagnosticAutoMode(xero::base::Robot &robot) ;
            virtual ~DiagnosticAutoMode() ;

        private:
        } ;

    }
}


