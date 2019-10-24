#pragma once

#include <oi/OIDevice.h> 
#include <SequenceAction.h>

namespace xero {
    namespace ranseur {
        class RanseurOISubsystem ;

        class RanseurOIDevice : public xero::base::OIDevice {
        public:
            RanseurOIDevice(RanseurOISubsystem &sub, int index) ;
            virtual ~RanseurOIDevice() ;

            virtual int getAutoModeSelector() {
                return getValue(automode_) ;
            }

            virtual void init() ;
            virtual void generateActions(xero::base::SequenceAction &seq) ;

        private:
            void bindOI() ;
        private:
            size_t automode_ ;
        } ;
    }
}