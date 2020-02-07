#pragma once

#include "DroidAutoMode.h"

namespace xero
{
    namespace droid
    {
        class DroidFiveBallAutomode: public DroidAutoMode
        {
        public:
            DroidFiveBallAutomode(xero::base::Robot &robot) ;
            virtual ~DroidFiveBallAutomode() ;
        } ;
    }
}