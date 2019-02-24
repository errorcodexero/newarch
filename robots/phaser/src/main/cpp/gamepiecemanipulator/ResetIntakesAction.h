#pragma once 

#include "GamePieceAction.h"
#include "GamePieceManipulator.h"

namespace xero{
    namespace phaser{
        class ResetIntakeAction : public GamePieceAction {
        public:
            ResetIntakeAction(GamePieceManipulator &subsystem) ;
            virtual ~ResetIntakeAction() ;

            virtual void start() ;
            virtual void run() ;
            virtual bool isDone() ;
            virtual void cancel() ;
            virtual std::string toString() ;
        private:

            enum class State {
                Lifting,
                Retracting,
                StopMotors,
                DropLift,
                Idle
            } ;

        private:
            State state_ ;
            xero::base::ActionPtr set_lifter_safe_height_ ;
            xero::base::ActionPtr set_lifter_level_one_ ;          
            xero::base::ActionPtr set_turntable_zero_ ;  
            xero::base::ActionPtr retract_cargo_intake_ ;
            xero::base::ActionPtr retract_hatch_intake_ ;
            xero::base::ActionPtr retract_arm_ ;

            xero::base::ActionPtr stop_hatch_intake_motor_ ;
            xero::base::ActionPtr stop_cargo_intake_motor_ ;
            xero::base::ActionPtr stop_cargo_holder_motor_ ;
        } ;
    }
}