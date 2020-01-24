#include "ConveyorPrepareToEmitAction.h"
#include "Conveyor.h"

namespace xero {
    namespace droid {
        void ConveyorPrepareToEmitAction::start() {
            // set motor power to move balls towards shooter
            getSubsystem().setMotor(1);
        }

        void ConveyorPrepareToEmitAction::run() {
            // if sensor detects {
                getSubsystem().setMotor(0); // stop
                setDone();
            // }
        }
    }
}