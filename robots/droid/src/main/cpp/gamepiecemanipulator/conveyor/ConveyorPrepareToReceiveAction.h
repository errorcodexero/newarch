#pragma once

#include "ConveyorStateAction.h"

namespace xero {
    namespace droid {
        /// An action which lines up balls for collecting.
        class ConveyorPrepareToReceiveAction : public ConveyorStateAction {
        public:
            ConveyorPrepareToReceiveAction(Conveyor &subsystem);
        } ;
    }
}