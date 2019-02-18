#pragma once 

#include "GamePieceAction.h"
#include "GamePieceManipulator.h"

namespace xero{
    namespace phaser{
        class ReadyCargoPlace : public GamePieceAction{
            ReadyCargoPlace(GamePieceManipulator &subsystem) ;
            virtual ~ReadyCargoPlace() ;

            virtual void start() ;
            virtual void run() ;
            virtual bool isDone() ;
            virtual void cancel() ;
            virtual std::string toString() ;
        
        private:
            enum class State{
                Idle,
                TurntableGoToAngle,
                LifterGoToScoreHeight
            } ;

        private:
            State state_ ;

            xero::base::ActionPtr set_lifter_score_height_ ;
        }
    }
}