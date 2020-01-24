#include "ConveyorPrepareToReceiveAction.h"
#include "Conveyor.h"

namespace xero {
    namespace droid {
        void ConveyorPrepareToReceiveAction::start() {
            // set motor power to move balls towards intake
            getSubsystem().setMotor(-1);
        }

        void ConveyorPrepareToReceiveAction::run() {
            // if sensor detects {
                getSubsystem().setMotor(0); // stop
                setDone();
            // }
        }
    }
}