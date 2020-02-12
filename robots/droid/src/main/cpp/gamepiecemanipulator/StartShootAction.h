#pragma once

#include "GamePieceManipulatorAction.h"

namespace xero {
    namespace droid {
        class StartShootAction : public GamePieceManipulatorAction {
        public:
            StartShootAction(GamePieceManipulator &subsystem);
            virtual ~StartShootAction();

            void start() override ;
            void run() override ;

            std::string toString() override {
                return "StartShootAction";
            }

        private:
        };
    }

}
