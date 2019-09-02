#pragma once

#include <Robot.h>
#include <AutoMode.h>

namespace xero {
    namespace gopigo3 {
        class LedAutoMode : public xero::base::AutoMode
        {
        public:
            LedAutoMode(xero::base::Robot &robot) ;
            virtual ~LedAutoMode() ;
        } ;
    }
}
