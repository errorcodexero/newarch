#include "ConveyorPrepareToReceiveAction.h"
#include "Conveyor.h"

namespace xero {
    namespace droid {
        ConveyorPrepareToReceiveAction::ConveyorPrepareToReceiveAction(Conveyor &subsystem):
            ConveyorStateAction(subsystem, "ConveyorPrepareToReceiveAction") {
            
            const std::string done = "done";
            const std::string setStaged = "set staged";
            setStates({
                // if full or already staged for collection, stop
                branchState(done, [=] { return getSubsystem().isFull(); }),
                branchState(setStaged, [=] { return getSubsystem().isStagedForCollect(); }),
                branchState(setStaged, [=] { return getSubsystem().isEmpty(); }),

                // Move balls to the ready-to-collect position
                setMotorState(MotorState::MoveTowardsIntake),
                { "wait for balls to reach end", waitForSensorState(Sensor::A, true) },
                
                setMotorState(MotorState::MoveTowardsShooter),
                { "wait for balls to clear end sensor", waitForSensorState(Sensor::A, false) },
                { "wait for balls to reach target sensor", waitForSensorState(Sensor::B, true) },
                
                { setStaged, [=]{
                    setStagedForCollect(true);
                    setStagedForFire(false);
                    setCollecting(false);
                    return StateResult::Next;
                }},
                
                { done, setMotorState(MotorState::Stopped) },
            });
        }
    }
}