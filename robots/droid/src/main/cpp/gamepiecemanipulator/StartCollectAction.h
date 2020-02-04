#pragma once

#include "GamePieceManipulatorAction.h"

namespace xero {
    namespace droid {
        class StartCollectAction : public GamePieceManipulatorAction {
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
