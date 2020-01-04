#pragma once

#include <oi/OIDevice.h> 
#include <actions/Action.h>
#include <actions/SequenceAction.h>

namespace xero {
    namespace droid {
        class droidOISubsystem ;

        class droidOIDevice : public xero::base::OIDevice {
        public:
            droidOIDevice(droidOISubsystem &sub, int index) ;
            virtual ~droidOIDevice() ;

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
