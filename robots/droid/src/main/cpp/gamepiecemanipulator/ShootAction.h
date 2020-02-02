#pragma once

#include "shooter/Shooter.h"
#include "intake/Intake.h"
#include "conveyor/Conveyor.h"
#include "gamepiecemanipulator/GamePieceManipulatorAction.h"

namespace xero {
    namespace droid {
        class ShootAction : GamePieceManipulatorAction {
        public:
            ShootAction(GamePieceManipulator &subsystem);
            virtual ~ShootAction();

            void start() override ;
            void run() override ;

            std::string toString() override {
                return "ShootAction";
            }
            
        private:
        };
    }

}
