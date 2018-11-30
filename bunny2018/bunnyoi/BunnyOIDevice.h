#pragma once

#include <OIDevice.h>
#include <Action.h>

namespace xero {
    namespace bunny2018 {
        class BunnyOISubsystem ;

        class BunnyOIDevice : public xero::base::OIDevice {
        public:
            BunnyOIDevice(BunnyOISubsystem &sub, int index) ;
            virtual ~BunnyOIDevice() ;

            virtual int getAutoModeSelector() {
                return getValue(automode_) ;
            }

            virtual void computeState(xero::base::ActionSequence &seq) ;

        private:
			void createActions() ;
            void initialize() ;
            
        private:
            size_t collector_on_off_ ;
            size_t hopper_on_off_ ;
            size_t automode_ ;

			xero::base::ActionPtr collector_on_action_ ;
			xero::base::ActionPtr collector_off_action_ ;
			xero::base::ActionPtr hopper_on_action_ ;
			xero::base::ActionPtr hopper_off_action_ ;									
        } ;
    }
}