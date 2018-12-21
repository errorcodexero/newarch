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

            virtual int getAutoModeSelector() ;

			virtual void generateActions(xero::base::ActionSequence &seq) ;

        private:
			void createActions() ;
            void initialize() ;
            
        private:
            size_t collector_ ;
            size_t hopper_ ;
            size_t shoot_one_ ;
            size_t shoot_many_ ;
            size_t automode_ ;
			size_t test_auto_modes_ ;
            size_t intake_on_off_ ;

			int base_ ;

            xero::base::ActionPtr collector_rev_action_ ;
			xero::base::ActionPtr collector_fwd_action_ ;
			xero::base::ActionPtr collector_off_action_ ;
			xero::base::ActionPtr hopper_rev_action_ ;
			xero::base::ActionPtr hopper_fwd_action_ ;
			xero::base::ActionPtr hopper_off_action_ ;	
            xero::base::ActionPtr sort_red_ ;
            xero::base::ActionPtr sort_blue_; 
            xero::base::ActionPtr shoot_one_action_ ;
            xero::base::ActionPtr shoot_many_action_on_ ;
            xero::base::ActionPtr shoot_many_action_stage_ ;
			xero::base::ActionPtr intake_on_action_ ;
			xero::base::ActionPtr intake_off_action_ ;	            
        } ;
    }
}