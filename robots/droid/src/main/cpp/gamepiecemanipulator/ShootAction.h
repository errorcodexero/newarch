#pragma once

#include "gamepiecemanipulator/GamePieceManipulatorAction.h"

namespace xero {
    namespace droid {
        class ShootAction : public GamePieceManipulatorAction {
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
