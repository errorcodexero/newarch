#pragma once

#include "GamePieceManipulatorAction.h"
#include <gamepiecemanipulator/shooter/FireAction.h>
#include <memory>

namespace xero {
    namespace droid {
        class ShootTestingAction : public GamePieceManipulatorAction {
        public:
            ShootTestingAction(GamePieceManipulator &subsystem);
            virtual ~ShootTestingAction();

            void start() override ;
            void run() override ;

            std::string toString() override {
                return "ShootTestingAction";
            }            
            
        private:
            std::shared_ptr<FireAction> fire_;
            frc::SimpleWidget widget_;            
        };
    }

}
