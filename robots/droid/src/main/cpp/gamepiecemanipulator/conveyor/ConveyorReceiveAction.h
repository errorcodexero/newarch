#pragma once

#include "ConveyorAction.h"

namespace xero {
    namespace droid {
        class ConveyorReceiveAction : public ConveyorAction {
        public:
            ConveyorReceiveAction(Conveyor &subsystem);
        } ;
    }
}