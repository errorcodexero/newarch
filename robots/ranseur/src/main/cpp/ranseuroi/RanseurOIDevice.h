#pragma once

#include <oi/OIDevice.h> 
#include <actions/SequenceAction.h>

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
            void initialize() ;
        private:
            size_t automode_ ;
            size_t collect_ ;
            size_t dump_ ;
            size_t eject_ ;
            size_t turtle_ ;
            size_t spare1_ ;
            size_t spare2_ ;
  
            xero::base::ActionPtr collecting_ ;
            xero::base::ActionPtr dumping_ ;
            xero::base::ActionPtr turtling_ ;
            xero::base::ActionPtr ejecting_ ;

        } ;
    }
}
