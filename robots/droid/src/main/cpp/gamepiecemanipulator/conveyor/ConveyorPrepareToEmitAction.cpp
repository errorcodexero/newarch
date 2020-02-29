#include "ConveyorPrepareToEmitAction.h"
#include "Conveyor.h"

namespace xero {
    namespace droid {
        ConveyorPrepareToEmitAction::ConveyorPrepareToEmitAction(Conveyor &subsystem):
            ConveyorStateAction(subsystem, "ConveyorPrepareToEmitAction") {
            
            const std::string done = "done";
            const std::string setStaged = "setStaged";
            setStates({
                // if empty or already staged for fire, stop
                branchState(done, [=] { return getSubsystem().isEmpty(); }),
                branchState(done, [=] { return getSubsystem().isStagedForFire(); }),
                branchState(setStaged, [=] { return getSubsystem().isFull(); }),

                
                // move balls in position towards shooter
                setMotorState(MotorState::MoveTowardsShooter),
                { "wait for ball to reach target sensor", waitForSensorEdgeState(Sensor::D, false) },
                
                {setStaged,  [=]{ 
                    setStagedForCollect(false); 
                    setStagedForFire(true);
                    setCollecting(false);
                    return StateResult::Next; 
                }},

                { done, setMotorState(MotorState::Stopped) },
            });
        }
    }
}