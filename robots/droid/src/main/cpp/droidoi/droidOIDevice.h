#pragma once

#include <oi/OIDevice.h> 
#include <actions/Action.h>
#include <actions/SequenceAction.h>

namespace xero {
    namespace droid {
        class DroidOISubsystem ;

        class DroidOIDevice : public xero::base::OIDevice {
        public:
            DroidOIDevice(DroidOISubsystem &sub, int index) ;
            virtual ~DroidOIDevice() ;

            virtual int getAutoModeSelector() {
                return getValue(automode_) ;
            }

            virtual void generateActions(xero::base::SequenceAction &seq) ;

            virtual void init() ;

        private:
            void bindOI() ;
            
        private:
            size_t automode_ ;
        } ;
    }
}
