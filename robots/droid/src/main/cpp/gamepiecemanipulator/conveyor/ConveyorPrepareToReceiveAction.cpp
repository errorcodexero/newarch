#include "ConveyorPrepareToReceiveAction.h"
#include "Conveyor.h"

namespace xero {
    namespace droid {
        ConveyorPrepareToReceiveAction::ConveyorPrepareToReceiveAction(Conveyor &subsystem):
            ConveyorAction(subsystem, "ConveyorPrepareToReceiveAction") {
            
            const std::string done = "done";
            setStates({
                // if full or empty, stop
                branchState(done, [=] { return getSubsystem().isFull(); }),
                branchState(done, [=] { return getSubsystem().isEmpty(); }),

                // Move balls to the ready-to-collect position
                setMotorState(MotorState::MoveTowardsIntake),
                waitForSensorState(Sensor::A, true),
                
                setMotorState(MotorState::MoveTowardsShooter),
                waitForSensorState(Sensor::A, false),
                waitForSensorState(Sensor::B, true),

                { done, setMotorState(MotorState::Stopped) },
            });
        }
    }
}