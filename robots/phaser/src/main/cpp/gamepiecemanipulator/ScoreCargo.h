#pragma once 

#include "GamePieceAction.h"
#include "GamePieceManipulator.h"

namespace xero{
    namespace phaser{
        class ScoreCargo : public GamePieceAction{
            ScoreCargo(GamePieceManipulator &subsystem) ;
            virtual ~ScoreCargo() ;

            virtual void start() ;
            virtual void run() ;
            virtual bool isDone() ;
            virtual void cancel() ;
            virtual std::string toString() ;
        
        private:
            enum class State{
                Idle,
                TurntableGoToAngle,
                LifterGoToScoreHeight,
                ScoreCargo,
                StopMotor
            } ;

        private:
            State state_ ;

            xero::base::ActionPtr set_lifter_score_height_ ;
            xero::base::ActionPtr hold_lifter_score_height_ ;
            xero::base::ActionPtr set_turntable_cargo_angle_ ;

            xero::base::ActionPtr set_cargo_holder_motor_ ;
            xero::base::ActionPtr stop_cargo_holder_motor_ ;
        } ;
    }
}