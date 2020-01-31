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
                { "wait for balls to reach end", waitForSensorState(Sensor::A, true) },
                
                setMotorState(MotorState::MoveTowardsShooter),
                { "wait for balls to clear end sensor", waitForSensorState(Sensor::A, false) },
                { "wait for balls to reach target sensor", waitForSensorState(Sensor::B, true) },

                { done, setMotorState(MotorState::Stopped) },
            });
        }
    }
}