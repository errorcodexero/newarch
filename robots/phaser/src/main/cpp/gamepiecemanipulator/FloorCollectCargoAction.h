#pragma once

#include <GamePieceAction.h>
#include <GamePieceManipulator.h>

namespace xero{
    namespace phaser{
        class FloorCollectAction : public GamePieceAction{
        public:
            FloorCollectAction() ;
            virtual ~FloorCollectAction() ;

            virtual void start() ;
            virtual void run() ;
            virtual bool isDone() ;
            virtual void cancel() ;
            virtual std::string toString() ;

        private:
            ActionPtr set_lifter_safe_height_ ; 
            ActionPtr set_turntable_cargo_angle_ ;
            ActionPtr set_lifter_cargo_intake_height_ ;

            ActionPtr deploy_cargo_intake_ ;
            ActionPtr retract_cargo_intake_ ;

            ActionPtr set_cargo_intake_motor_ ;
            ActionPtr stop_cargo_intake_motor_ ;
            ActionPtr set_cargo_holder_motors_ ;
            ActionPtr stop_cargo_holder_motors_ ;
            
        };
    }
}