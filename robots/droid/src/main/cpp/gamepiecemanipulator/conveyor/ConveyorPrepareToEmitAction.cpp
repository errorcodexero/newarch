#include "ConveyorPrepareToEmitAction.h"
#include "Conveyor.h"

namespace xero {
    namespace droid {
        void ConveyorPrepareToEmitAction::start() {
            if (getSubsystem().ballCount_ == 0) {
                setDone();
            }
        }

        void ConveyorPrepareToEmitAction::run() {
            getSubsystem().setMotor(1);    // move balls toward shooter
            if (/* sensor C is tripped */true) {
                // balls are in position next to shooter
                setDone();
            }
        }
    }
}