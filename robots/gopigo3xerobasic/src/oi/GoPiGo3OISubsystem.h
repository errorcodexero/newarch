#pragma once

#include <oi/OISubsystem.h>

namespace xero {
    namespace gopigo3 {
        class GoPiGo3OISubsystem : public xero::base::OISubsystem {
        public:
            GoPiGo3OISubsystem(xero::base::Robot &robot) ;
            virtual ~GoPiGo3OISubsystem() ;
            
        private:
        } ;
    }
}