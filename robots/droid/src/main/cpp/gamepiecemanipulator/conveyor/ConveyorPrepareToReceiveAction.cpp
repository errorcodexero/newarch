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