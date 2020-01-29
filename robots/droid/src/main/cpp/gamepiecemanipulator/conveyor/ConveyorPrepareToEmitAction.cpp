#include "ConveyorPrepareToEmitAction.h"
#include "Conveyor.h"

namespace xero {
    namespace droid {
        ConveyorPrepareToEmitAction::ConveyorPrepareToEmitAction(Conveyor &subsystem):
            ConveyorAction(subsystem, "ConveyorPrepareToEmitAction") {
            
            const std::string done = "done";
            setStates({
                // if empty, stop
                branchState(done, std::bind(&Conveyor::isEmpty, getSubsystem())),
                
                // move balls in position towards shooter
                setMotorState(Direction::TowardsShooter),
                waitForSensorState(Sensor::C, true),

                { done, setMotorState(std::nullopt) },
            });
        }
    }
}