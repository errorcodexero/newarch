#pragma once

#include <HIDDevice.h>

namespace xero {
    namespace phoenix {
        class PhoenixOISubsystem ;
        
        class PhoenixGunnerGamepad : public xero::base::HIDDevice {
        public:
            PhoenixGunnerGamepad(PhoenixOISubsystem &oi, int index) ;
            virtual ~PhoenixGunnerGamepad() ;

            virtual void computeState(xero::base::ActionSequence &seq) ;

            int getAutoModeSelector() {
                return 0 ;
            }
        private:
        } ;
    }
}