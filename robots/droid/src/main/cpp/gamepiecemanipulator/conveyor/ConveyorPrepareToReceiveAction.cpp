#include "ConveyorPrepareToReceiveAction.h"
#include "Conveyor.h"

namespace xero {
    namespace droid {
        ConveyorPrepareToReceiveAction::ConveyorPrepareToReceiveAction(Conveyor &subsystem):
            ConveyorAction(subsystem, "ConveyorPrepareToReceiveAction") {
            
            const std::string done = "done";
            setStates({
                // if full, stop
                branchState(done, std::bind(&Conveyor::isFull, getSubsystem())),

                // Move balls to the ready-to-collect position
                setMotorState(Direction::TowardsIntake),
                waitForSensorState(Sensor::A, true),
                
                setMotorState(Direction::TowardsShooter),
                waitForSensorState(Sensor::A, false),
                waitForSensorState(Sensor::B, true),

                { done, setMotorState(std::nullopt) },
            });
        }
    }
}