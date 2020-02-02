#pragma once

#include "GamePieceManipulatorAction.h"

namespace xero {
    namespace droid {
        class StopCollectAction : public GamePieceManipulatorAction {
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
