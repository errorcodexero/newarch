#pragma once

#include <oi/OIDevice.h> 
#include <actions/Action.h>
#include <actions/SequenceAction.h>
//#include <queue>

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
            enum class CollectShootMode
            {
                CollectMode,
                ShootMode,
                InvalidMode
            } ;

            void initialize() ;
            CollectShootMode getSwitchMode() ;

        private:
            CollectShootMode flag_coll_v_shoot_ ;
            bool flag_collect_ ;
            bool flag_shoot_ ;
            
            //std::queue<xero::base::ActionPtr> conveyorActionQueue ;

            size_t automode_ ;

            size_t coll_v_shoot_ ;
            size_t collect_ ;
            size_t shoot_ ;

            xero::base::ActionPtr queue_prep_collect_ ;
            xero::base::ActionPtr queue_prep_shoot_ ;
            xero::base::ActionPtr fire_yes_ ;
            xero::base::ActionPtr turret_follow_ ;
            xero::base::ActionPtr intake_on_ ;
            xero::base::ActionPtr conveyor_receive_ ; 
            xero::base::ActionPtr start_collect_action_ ;
            xero::base::ActionPtr stop_collect_action_ ;
            xero::base::ActionPtr stop_shoot_action_ ;
    
        } ;
    }
}
