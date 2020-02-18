#include "ConveyorEjectAction.h"
#include "Conveyor.h"

namespace xero {
    namespace droid {
        ConveyorEjectAction::ConveyorEjectAction(Conveyor &subsystem):
            ConveyorStateAction(subsystem, "ConveyorEjectAction") {
            
            setStates({
                // move balls in position towards shooter
                setMotorState(MotorState::MoveTowardsIntake),
                { [=]{
                    setStagedForCollect(false);
                    setStagedForFire(false);
                    getSubsystem().ballCount_ = 0;
                    return StateResult::Next;
                }},
                delayState(3),
                setMotorState(MotorState::Stopped),
            });
        }
    }
}