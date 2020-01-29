#pragma once

#include "ConveyorAction.h"

namespace xero {
    namespace droid {
        /// An action which collects balls until full.
        class ConveyorReceiveAction : public ConveyorAction {
        public:
            ConveyorReceiveAction(Conveyor &subsystem);
        } ;
    }
}