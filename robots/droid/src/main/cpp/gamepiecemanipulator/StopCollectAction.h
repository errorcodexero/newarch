#pragma once

#include "GamePieceManipulatorAction.h"
#include "actions/ParallelAction.h"

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
            xero::base::ActionPtr collect_off_action_ ;
            xero::base::ActionPtr conveyor_stop_action_ ;
            
            xero::base::ParallelAction parallel_ ;
        };
    }

}
