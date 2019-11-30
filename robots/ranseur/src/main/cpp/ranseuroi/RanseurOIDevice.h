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

            virtual int getAutoModeSelector() ;
            
            virtual void init() ;
            virtual void generateActions(xero::base::SequenceAction &seq) ;

        private:
            void initialize() ;
        private:

#ifdef RANSEUR_OLD_OI
            size_t automode_ ;
#else
            size_t automode1_ ;
            size_t automode2_ ;
#endif            
            
            size_t collect_ ;
            size_t dump_ ;
            size_t eject_ ;
            size_t turtle_ ;
            size_t tubtoucher_ ;
            size_t spare2_ ;
  
            xero::base::ActionPtr collecting_ ;
            xero::base::ActionPtr dumping_ ;
            xero::base::ActionPtr turtling_ ;
            xero::base::ActionPtr ejecting_ ;
            xero::base::SequenceActionPtr sequence_ ;

        } ;
    }
}
