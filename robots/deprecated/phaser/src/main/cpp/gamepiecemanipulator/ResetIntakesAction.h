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
                GoToHeight,
                GoToAngle,
                WaitForIntake,                
                Idle
            } ;

        private:
            constexpr static double epsilon = 5.0 ;

        private:
            State state_ ;
            xero::base::ActionPtr retract_cargo_intake_ ;
            xero::base::ActionPtr retract_arm_ ;

            xero::base::ActionPtr ready_action_ ;

            xero::base::ActionPtr stop_cargo_intake_motor_ ;
            xero::base::ActionPtr stop_cargo_holder_motor_ ;

            xero::base::ActionPtr goto_height_ ;
            xero::base::ActionPtr goto_angle_ ;
        } ;
    }
}