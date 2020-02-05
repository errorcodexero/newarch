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

            virtual void init() ;

            virtual int getAutoModeSelector() ;

            virtual void generateActions(xero::base::SequenceAction &seq) ;
        
        private:
            void initialize() ;

        private:
            size_t automode_ ;

            size_t intake_ ;
            size_t queue_ ;
            size_t shoot_ ;

            xero::base::ActionPtr intake_collect_ ;
            xero::base::ActionPtr intake_retract_ ;
            xero::base::ActionPtr queue_collect_ ;
            xero::base::ActionPtr queue_shoot_ ;
            xero::base::ActionPtr fire_yes_ ;
            //xero::base::ActionPtr fire_no_ ;

        } ;
    }
}
