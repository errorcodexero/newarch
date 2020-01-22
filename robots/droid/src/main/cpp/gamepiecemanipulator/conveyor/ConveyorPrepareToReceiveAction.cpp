#include "ConveyorPrepareToReceiveAction.h"
#include "Conveyor.h"

namespace xero {
    namespace droid {
        void ConveyorPrepareToReceiveAction::start() {
            // set motor power to move balls to front
        }

        void ConveyorPrepareToReceiveAction::run() {
            // if sensor detects {
                getSubsystem().setMotor(-1);
                setDone();
            // }
        }
    }
}