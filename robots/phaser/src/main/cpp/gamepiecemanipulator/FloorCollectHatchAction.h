#pragma once

#include "GamePieceAction.h"
#include "GamePieceManipulator.h"

namespace xero{
    namespace phaser{
        class FloorCollectHatchAction : public GamePieceAction{
        public:
            FloorCollectHatchAction(GamePieceManipulator &subsystem) ;
            virtual ~FloorCollectHatchAction() ;

            virtual void start() ;
            virtual void run() ;
            virtual bool isDone() ;
            virtual void cancel() ;
            virtual std::string toString() ;

        private:
            enum class State {
                Idle,
                LifterGoToSafeHeight,
                TurntableGoToCollectAngle,
                LifterGoToCollectHeightDeployIntakeRunMotor,
                WaitForHatch,
                FlipIntake,
                RaiseLiftFromIntake, 
                DeployFinger
                // another state for time to stop motor may be needed for tuning
            } ;

        private:
            State state_ ;

            xero::base::ActionPtr set_lifter_safe_height_ ; 
            xero::base::ActionPtr set_turntable_hatch_angle_ ;
            xero::base::ActionPtr set_lifter_hatch_intake_height_ ;
            xero::base::ActionPtr set_lifter_undock_hatch_height_ ;

            xero::base::ActionPtr set_extend_arm_ ;
            xero::base::ActionPtr set_retract_arm_ ;
            xero::base::ActionPtr set_deploy_hatch_finger_ ;
            xero::base::ActionPtr set_retract_hatch_finger_ ;

            xero::base::ActionPtr deploy_hatch_intake_ ;
            xero::base::ActionPtr retract_hatch_intake_ ;

            xero::base::ActionPtr set_hatch_intake_motor_ ;
            xero::base::ActionPtr stop_hatch_intake_motor_ ;
            xero::base::ActionPtr rumble_ ;            
        };
    }
}