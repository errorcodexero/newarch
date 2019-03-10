#pragma once

#include "GamePieceAction.h"
#include "GamePieceManipulator.h"

namespace xero {
    namespace phaser {
        class FloorCollectCargoAction : public GamePieceAction {
        public:
            FloorCollectCargoAction(GamePieceManipulator &subsystem) ;
            virtual ~FloorCollectCargoAction() ;

            virtual void start() ;
            virtual void run() ;
            virtual bool isDone() ;
            virtual void cancel() ;
            virtual void abort() ;
            virtual std::string toString() ;

            virtual void reverseIntake() ;

        private:
            enum class State {
                Idle,
                LifterGoToSafeHeight,
                TurntableGoToCollectAngle,
                LifterGoToCollectHeightDeployIntake,
                WaitForCargo,
                WaitForCargo2,
                StopAllMotors,
                RaiseLifter,
                RetractIntake,
            } ;

        private:
            State state_ ;

            bool retracted_intake_ ;

            xero::base::ActionPtr set_lifter_safe_height_ ; 
            xero::base::ActionPtr set_turntable_cargo_angle_ ;
            xero::base::ActionPtr set_lifter_cargo_intake_height_ ;
            xero::base::ActionPtr set_lifter_cargo_collected_height_ ;

            xero::base::ActionPtr deploy_cargo_intake_ ;
            xero::base::ActionPtr retract_cargo_intake_ ;

            xero::base::ActionPtr set_cargo_intake_motor_ ;
            xero::base::ActionPtr stop_cargo_intake_motor_ ;
            xero::base::ActionPtr set_cargo_holder_motor_ ;
            xero::base::ActionPtr stop_cargo_holder_motor_ ;

            xero::base::ActionPtr reverse_cargo_intake_motor_ ;
            
            xero::base::ActionPtr rumble_ ;          

            bool reversed_ ;  
        };
    }
}