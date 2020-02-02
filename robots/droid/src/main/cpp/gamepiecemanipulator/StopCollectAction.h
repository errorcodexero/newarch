#pragma once

#include "GamePieceManipulatorAction.h"

#include "shooter/Shooter.h"
#include "intake/Intake.h"
#include "conveyor/Conveyor.h"

namespace xero {
    namespace droid {
        class StopCollectAction : GamePieceManipulatorAction {
        public:
            StopCollectAction(GamePieceManipulator &subsystem);
            virtual ~StopCollectAction();

            void start() override ;
            void run() override ;

            std::string toString() override {
                return "StopCollectAction";
            }            
            
        private:
        };
    }

}
