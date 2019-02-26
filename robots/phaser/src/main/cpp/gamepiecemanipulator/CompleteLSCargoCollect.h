#pragma once

#include "GamePieceAction.h"
#include "GamePieceManipulator.h"

namespace xero{
    namespace phaser{
        class CompleteLSCargoCollect : public GamePieceAction{
        public:
            CompleteLSCargoCollect(GamePieceManipulator &subsystem) ;
            virtual ~CompleteLSCargoCollect() ;

            virtual void start() ;
            virtual void run() ;
            virtual bool isDone() ;
            virtual void cancel() ;
            virtual void abort() ;
            virtual std::string toString() ;

        private:
            enum class State {
                Idle,
                WaitForCargo,
                StopAllMotors
            } ;

        private:
            State state_ ;

            xero::base::ActionPtr set_cargo_holder_motor_ ;
            xero::base::ActionPtr stop_cargo_holder_motor_ ;
            
        };
    }
}