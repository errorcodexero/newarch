#pragma once 

#include "GamePieceAction.h"
#include "GamePieceManipulator.h"
#include "phasercameratracker/PhaserCameraTracker.h"

namespace xero{
    namespace phaser{
        class CargoShootAction : public GamePieceAction {
        public:
            CargoShootAction(GamePieceManipulator &subsystem, PhaserCameraTracker &vision) ;
            virtual ~CargoShootAction() ;

            virtual void start() ;
            virtual void run() ;
            virtual bool isDone() ;
            virtual void cancel() ;
            virtual std::string toString() ;

        private:
            enum class State {
                Rotating,
                Shooting,
                Idle,
            } ;

        private:
            PhaserCameraTracker &vision_ ;
            State state_ ;

            double width_ ;
            
            xero::base::ActionPtr set_cargo_holder_motor_ ;
            xero::base::ActionPtr stop_cargo_holder_motor_ ;
            xero::base::ActionPtr rumble_ ;
            xero::base::ActionPtr rotate_turntable_ ;
        } ;
    }
}