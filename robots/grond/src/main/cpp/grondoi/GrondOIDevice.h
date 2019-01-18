#pragma once

#include <OIDevice.h>
#include <Action.h>

namespace xero {
    namespace grond {
        class GrondOISubsystem ;

        class GrondOIDevice : public xero::base::OIDevice {
        public:
            GrondOIDevice(GrondOISubsystem &sub, int index) ;
            virtual ~GrondOIDevice() ;

            virtual int getAutoModeSelector() {
                return getValue(automode_) ;
            }

            virtual void generateActions(xero::base::ActionSequence &seq) ;

        private:
            void createActions() ;
            void initialize() ;
            
        private:
            uint32_t automode_ ;        
        } ;
    }
}