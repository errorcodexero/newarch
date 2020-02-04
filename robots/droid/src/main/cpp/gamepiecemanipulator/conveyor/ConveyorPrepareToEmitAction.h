#pragma once

#include "ConveyorStateAction.h"

namespace xero {
    namespace droid {
        /// An action which lines up balls for firing.
        class ConveyorPrepareToEmitAction : public ConveyorStateAction {
        public:
            ConveyorPrepareToEmitAction(Conveyor &subsystem);
        } ;
    }
}