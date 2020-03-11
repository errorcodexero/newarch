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
            enum class CollectShootState
            {
                PreparingForCollect,
                FinishingCollect,
                WaitForIntake,
                CollectReady,
                Collecting,
                PreparingForShoot,
                ShootReady,
                Ejecting,  
                InvalidMode
            } ;

            void initialize() ;
            bool isCollectButtonPressed() ;

            void generateCollectShootActions(xero::base::SequenceAction &seq) ;
            void generateClimbActions(xero::base::SequenceAction &seq) ;
            void generatePanelSpinnerActions(xero::base::SequenceAction &seq) ;


            void shootMode(xero::base::SequenceAction &seq) ;
            void collectMode(xero::base::SequenceAction &seq) ;
            void startEject(xero::base::SequenceAction &seq) ;
            void stopEject(xero::base::SequenceAction &seq) ;

            void processPreparingForCollect(xero::base::SequenceAction &seq) ;
            void processFinishingCollect(xero::base::SequenceAction &seq) ;
            void processWaitingForIntake(xero::base::SequenceAction &seq) ;
            void processCollectReady(xero::base::SequenceAction &seq) ;            
            void processCollecting(xero::base::SequenceAction &seq) ;
            void processPrepareForShoot(xero::base::SequenceAction &seq) ;
            void processShootReady(xero::base::SequenceAction &seq) ;
            void processShooting(xero::base::SequenceAction &seq) ;
            void processEjecting(xero::base::SequenceAction &seq) ;
            void processInvalidMode(xero::base::SequenceAction &seq) ;

            std::string toString(CollectShootState v) ;

        private:
            enum SpinnerState
            {
                Start,
                WaitingForIntakeDown,
                RaisingArm,
                Raised,
                LoweringArm,                
                WaitingForIntakeUp
            } ;

        private:
            CollectShootState shoot_collect_state_ ;

            bool started_deploy_;
            bool climber_deployed_ ;

            bool controlPanelDeployed_;
            bool controlPanelActive_;
            
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
            size_t manual_shoot_ ;
            size_t manual_mode_ ;

            size_t spin_deploy_ ;
            double start_time_ ;

            bool rumbled_ ;

            size_t control_panel_servo_;
            size_t control_panel_rotation_;
            size_t control_panel_position_;

            xero::base::ActionPtr queue_prep_collect_ ;
            xero::base::ActionPtr queue_prep_shoot_ ;
            xero::base::ActionPtr fire_yes_ ;
            xero::base::ActionPtr turret_follow_ ;
            xero::base::ActionPtr intake_on_ ;
            xero::base::ActionPtr intake_off_;
            xero::base::ActionPtr conveyor_receive_ ; 
            xero::base::ActionPtr start_collect_action_ ;
            xero::base::ActionPtr stop_collect_action_ ;
            xero::base::ActionPtr stop_shoot_action_ ;
            xero::base::ActionPtr turret_goto_zero_ ;
            xero::base::ActionPtr hood_down_;
            xero::base::ActionPtr eject_action_;
            xero::base::ActionPtr shooter_eject_action_;
            xero::base::ActionPtr shooter_spinup_;
            xero::base::ActionPtr shooter_stop_ ;
    
            xero::base::ActionPtr control_panel_up_;
            xero::base::ActionPtr control_panel_down_;
            xero::base::ActionPtr control_panel_position_action_;
            xero::base::ActionPtr control_panel_rotation_action_;

            xero::base::ActionPtr deploy_climber_ ;
            xero::base::ActionPtr stop_ ;      

            SpinnerState spinner_state_ ;               
        } ;
    }
}
