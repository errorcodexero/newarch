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
            void cancel() override ;
            
            std::string toString() override {
                return "StopCollectAction";
            }            

        private:
            xero::base::ActionPtr conveyor_stop_action_ ;
            bool started_;
            double timeoutEnd_;
        };
    }

}
