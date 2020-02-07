#pragma once

#include "DroidAutoMode.h"

namespace xero
{
    namespace droid
    {
        class DroidThreeBallAutomode: public DroidAutoMode
        {
        public:
            DroidThreeBallAutomode(xero::base::Robot &robot) ;
            virtual ~DroidThreeBallAutomode() ;
        } ;
    }
}