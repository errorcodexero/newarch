#pragma once

#include "ConveyorStateAction.h"

namespace xero {
    namespace droid {
        /// An action which ejects all balls through the intake.
        class ConveyorEjectAction : public ConveyorStateAction {
        public:
            ConveyorEjectAction(Conveyor &subsystem);
        } ;
    }
}