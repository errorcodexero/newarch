#pragma once

#include "DroidAutoMode.h"

namespace xero
{
    namespace droid
    {
        class DroidTenAutomode: public DroidAutoMode
        {
        public:
            DroidTenAutomode(xero::base::Robot &robot) ;
            virtual ~DroidTenAutomode() ;
        } ;
    }
}