#pragma once

#include "ConveyorAction.h"

namespace xero {
    namespace droid {
        /// An action which emits a single ball.
        class ConveyorEmitAction : public ConveyorAction {
        public:
            ConveyorEmitAction(Conveyor &subsystem);
        } ;
    }
}