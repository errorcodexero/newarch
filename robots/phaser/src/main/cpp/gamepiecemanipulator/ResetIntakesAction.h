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
                Idle
            } ;

        private:
            State state_ ;
            xero::base::ActionPtr set_lifter_safe_height_ ;
            xero::base::ActionPtr hold_lifter_safe_height_ ;
            xero::base::ActionPtr retract_cargo_intake_ ;
            xero::base::ActionPtr retract_hatch_intake_ ;
        } ;
    }
}