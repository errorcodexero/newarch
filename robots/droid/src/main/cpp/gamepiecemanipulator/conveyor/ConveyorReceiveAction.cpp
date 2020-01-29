#include "ConveyorReceiveAction.h"
#include "Conveyor.h"

namespace xero {
    namespace droid {
        ConveyorReceiveAction::ConveyorReceiveAction(Conveyor &subsystem):
            ConveyorAction(subsystem, "ConveyorReceiveAction") {
            
            const std::string waitForBall = "waitForBall";
            const std::string done = "done";
            setStates({
                { waitForBall, setMotorState(std::nullopt) },
                // if full, stop
                branchState(done, std::bind(&Conveyor::isFull, getSubsystem())),

                assertState([=]() { return getSubsystem().isEmpty() 
                                        || getSubsystem().readSensor(Sensor::B);
                            },  "ConveyorReceiveAction called with balls out of position; "
                                "was ConveyorPrepareToEmitAction run?"
                ),
                
                waitForSensorState(Sensor::A, true),
                
                // we've got a ball
                incrementBallsState(),
                setMotorState(Direction::TowardsShooter),
                waitForSensorState(Sensor::B, true),

                // ball collected, now collect another
                gotoState(waitForBall),

                { done, setMotorState(std::nullopt) },
            });
        }
    }
}
