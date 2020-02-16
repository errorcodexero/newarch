#pragma once

#include "GamePieceManipulatorAction.h"
#include "actions/ParallelAction.h"

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
            //xero::base::SequenceActionPtr sequence_ ;
            xero::base::ActionPtr collect_off_action_ ;
            xero::base::ActionPtr fire_action_ ;
        };

    }

}
