#pragma once

#include "GamePieceManipulatorAction.h"

namespace xero {
    namespace droid {
        class StopShootAction : public GamePieceManipulatorAction {
        public:
            StopShootAction(GamePieceManipulator &subsystem);
            virtual ~StopShootAction();

            void start() override ;
            void run() override ;

            std::string toString() override {
                return "StopShootAction";
            }            
            
        private:
        };
    }

}
