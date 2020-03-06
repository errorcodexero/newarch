#pragma once

#include "ConveyorStateAction.h"

namespace xero {
    namespace droid {
        /// An action which collects balls until full.
        class ConveyorReceiveAction : public ConveyorStateAction {
        public:
            ConveyorReceiveAction(Conveyor &subsystem);

            void conveyorActionStarted() override;
            void conveyorActionRun() override;
        private:
            bool collecting_;
            bool finishing_;
            bool reachedShooterSensor_;
        } ;
    }
}
