#pragma once

#include "GamePieceManipulatorAction.h"

#include "shooter/Shooter.h"
#include "intake/Intake.h"
#include "conveyor/Conveyor.h"

namespace xero {
    namespace droid {
        class StartCollectAction : GamePieceManipulatorAction {
        public:
            StartCollectAction(GamePieceManipulator &subsystem);
            virtual ~StartCollectAction();

            void start() override ;
            void run() override ;

            std::string toString() override {
                return "StartCollectAction";
            }

        private:
        };
    }

}
