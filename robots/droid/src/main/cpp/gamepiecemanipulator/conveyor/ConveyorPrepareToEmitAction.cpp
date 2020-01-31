#include "ConveyorPrepareToEmitAction.h"
#include "Conveyor.h"

namespace xero {
    namespace droid {
        ConveyorPrepareToEmitAction::ConveyorPrepareToEmitAction(Conveyor &subsystem):
            ConveyorAction(subsystem, "ConveyorPrepareToEmitAction") {
            
            const std::string done = "done";
            setStates({
                // if empty, stop
                branchState(done, [=] { return getSubsystem().isEmpty(); }),
                
                // move balls in position towards shooter
                setMotorState(MotorState::MoveTowardsShooter),
                waitForSensorState(Sensor::C, true),

                { done, setMotorState(MotorState::Stopped) },
            });
        }
    }
}