#pragma once

#include "ConveyorAction.h"

namespace xero {
    namespace droid {
        /// An action which lines up balls for collecting.
        class ConveyorPrepareToReceiveAction : public ConveyorAction {
        public:
            ConveyorPrepareToReceiveAction(Conveyor &subsystem);
        } ;
    }
}