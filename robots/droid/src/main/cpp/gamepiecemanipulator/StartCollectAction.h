#pragma once

#include "GamePieceManipulatorAction.h"
#include "actions/ParallelAction.h"

namespace xero {
    namespace droid {
        class StartCollectAction : public GamePieceManipulatorAction {
        public:
            StartCollectAction(GamePieceManipulator &subsystem);
            virtual ~StartCollectAction();

            void start() override ;
            void run() override ;
            void cancel() override ;

            std::string toString() override {
                return "StartCollectAction";
            }

        private:
            //xero::base::SequenceActionPtr sequence_ ;
            xero::base::ActionPtr collect_on_action_ ;
            xero::base::ActionPtr conveyor_receive_action_ ;
            
            xero::base::ParallelAction parallel_ ;

        };
    }

}
