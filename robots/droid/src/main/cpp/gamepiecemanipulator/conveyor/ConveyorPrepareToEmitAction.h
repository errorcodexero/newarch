#pragma once

#include "ConveyorAction.h"

namespace xero {
    namespace droid {
        /// An action which lines up balls for firing.
        class ConveyorPrepareToEmitAction : public ConveyorAction {
        public:
            ConveyorPrepareToEmitAction(Conveyor &subsystem);
        } ;
    }
}