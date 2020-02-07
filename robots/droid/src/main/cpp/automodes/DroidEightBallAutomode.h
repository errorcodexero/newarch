#pragma once

#include "DroidAutoMode.h"

namespace xero
{
    namespace droid
    {
        class DroidEightBallAutomode: public DroidAutoMode
        {
        public:
            DroidEightBallAutomode(xero::base::Robot &robot) ;
            virtual ~DroidEightBallAutomode() ;
        } ;
    }
}