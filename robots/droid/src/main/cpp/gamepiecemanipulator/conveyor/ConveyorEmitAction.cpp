#include "ConveyorEmitAction.h"
#include "Conveyor.h"

using namespace xero::misc;

namespace xero {
    namespace droid {
        ConveyorEmitAction::ConveyorEmitAction(Conveyor &subsystem):
            ConveyorAction(subsystem, "ConveyorEmitAction") {
            
            const std::string done = "done";
            setStates({
                // if empty, stop
                branchState(done, [=] { return getSubsystem().isEmpty(); }),

                assertState([=] { return getSubsystem().readSensor(Sensor::C); },
                            "ConveyorEmitAction called with no ball in position; "
                            "was ConveyorPrepareToEmitAction run?"
                ),
                
                // move a ball towards the shooter
                setMotorState(MotorState::MoveTowardsShooter),
                waitForSensorState(Sensor::C, false),
                
                decrementBallsState(),
                // if empty, stop
                branchState(done, [=] { return getSubsystem().isEmpty(); }),

                // wait for the next ball to move into position
                waitForSensorState(Sensor::C, true),

                // we're done
                { done, setMotorState(MotorState::Stopped) },
            });
        }
    }
} 