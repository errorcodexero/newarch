#pragma once

#include "ConveyorStateAction.h"

namespace xero {
    namespace droid {
        /// An action which emits a single ball.
        class ConveyorEmitAction : public ConveyorStateAction {
        public:
            ConveyorEmitAction(Conveyor &subsystem);

            void start() override;
            
            // Stops the action after the current shot completes.
            void stopFiring() { shouldStopFiring_ = true; }
        private:
            bool shouldStopFiring_;
        } ;
    }
}