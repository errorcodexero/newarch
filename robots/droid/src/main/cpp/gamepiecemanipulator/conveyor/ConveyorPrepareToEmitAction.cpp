#include "ConveyorPrepareToEmitAction.h"
#include "Conveyor.h"

namespace xero {
    namespace droid {
        ConveyorPrepareToEmitAction::ConveyorPrepareToEmitAction(Conveyor &subsystem):
            ConveyorStateAction(subsystem, "ConveyorPrepareToEmitAction") {
            
            const std::string done = "done";
            setStates({
                // if empty, stop
                branchState(done, [=] { return getSubsystem().isEmpty(); }),
                
                // move balls in position towards shooter
                setMotorState(MotorState::MoveTowardsShooter),
                { "wait for ball to reach target sensor", waitForSensorState(Sensor::D, true) },

                { "delay for fine-tuning", delayState(0.05) },

                { done, setMotorState(MotorState::Stopped) },
            });
        }
    }
}