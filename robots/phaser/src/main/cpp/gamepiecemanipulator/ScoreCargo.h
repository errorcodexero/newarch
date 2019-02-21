#pragma once 

#include "GamePieceAction.h"
#include "GamePieceManipulator.h"

namespace xero{
    namespace phaser{
        class ScoreCargo : public GamePieceAction {
        public:
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
                ScoreCargo,
            } ;

        private:
            State state_ ;

            xero::base::ActionPtr set_cargo_holder_motor_ ;
            xero::base::ActionPtr stop_cargo_holder_motor_ ;
        } ;
    }
}