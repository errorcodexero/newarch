#include "ConveyorAction.h"
#include "Conveyor.h"

#include <Robot.h>

namespace xero {
    namespace droid {
        ConveyorAction::ConveyorAction(Conveyor &subsystem):
            Action(subsystem.getRobot().getMessageLogger()), subsystem_(subsystem) {}
    }
}