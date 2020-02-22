#include "ConveyorPrepareToEmitAction.h"
#include "Conveyor.h"

namespace xero {
    namespace droid {
        ConveyorPrepareToEmitAction::ConveyorPrepareToEmitAction(Conveyor &subsystem):
            ConveyorStateAction(subsystem, "ConveyorPrepareToEmitAction") {
            
            const std::string done = "done";
            setStates({
                // if empty or already staged for fire, stop
                branchState(done, [=] { return getSubsystem().isEmpty(); }),
                branchState(done, [=] { return getSubsystem().isStagedForFire(); }),

                
                // move balls in position towards shooter
                setMotorState(MotorState::MoveTowardsShooter),
                { "wait for ball to reach target sensor", waitForSensorEdgeState(Sensor::D, false) },
                
                { [=]{ 
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