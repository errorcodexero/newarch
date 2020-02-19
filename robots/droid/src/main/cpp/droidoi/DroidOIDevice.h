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
            bool isCollectButtonPressed() ;

            void generateCollectShootActions(xero::base::SequenceAction &seq) ;
            void generateClimbActions(xero::base::SequenceAction &seq) ;
            void generatePanelSpinnerActions(xero::base::SequenceAction &seq) ;

        private:
            CollectShootMode flag_coll_v_shoot_ ;
            bool flag_collect_ ;
            bool flag_eject_;

            bool climber_deployed_ ;
            
            size_t automode_ ;

            size_t coll_v_shoot_ ;
            size_t collect_ ;
            size_t eject_;
            size_t climb_lock_ ;
            size_t climb_deploy_ ;
            size_t climb_secure_ ;
            size_t climb_up_ ;
            size_t climb_down_ ;
            size_t climb_left_ ;
            size_t climb_right_ ;

            xero::base::ActionPtr queue_prep_collect_ ;
            xero::base::ActionPtr queue_prep_shoot_ ;
            xero::base::ActionPtr fire_yes_ ;
            xero::base::ActionPtr turret_follow_ ;
            xero::base::ActionPtr intake_on_ ;
            xero::base::ActionPtr conveyor_receive_ ; 
            xero::base::ActionPtr start_collect_action_ ;
            xero::base::ActionPtr stop_collect_action_ ;
            xero::base::ActionPtr stop_shoot_action_ ;
            xero::base::ActionPtr turret_goto_zero_ ;
            xero::base::ActionPtr hood_down_;
            xero::base::ActionPtr eject_action_;
            xero::base::ActionPtr shooter_eject_action_;

            xero::base::ActionPtr deploy_climber_ ;
            xero::base::ActionPtr up_ ;
            xero::base::ActionPtr down_ ;
            xero::base::ActionPtr left_ ;
            xero::base::ActionPtr right_ ;
            xero::base::ActionPtr stop_ ;
            xero::base::ActionPtr lock_climber_ ;                        
        } ;
    }
}
