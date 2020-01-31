#include "ConveyorReceiveAction.h"
#include "Conveyor.h"

namespace xero {
    namespace droid {
        ConveyorReceiveAction::ConveyorReceiveAction(Conveyor &subsystem):
            ConveyorAction(subsystem, "ConveyorReceiveAction") {
            
            const std::string waitForBall = "waitForBall";
            const std::string done = "done";
            setStates({
                { waitForBall, setMotorState(MotorState::Stopped) },
                // if full, stop
                branchState(done, [=] { return getSubsystem().isFull(); } ),

                assertState([=]() { return getSubsystem().isEmpty() 
                                        || getSubsystem().readSensor(Sensor::B);
                            },  "ConveyorReceiveAction called with balls out of position; "
                                "was ConveyorPrepareToEmitAction run?"
                ),
                
                waitForSensorState(Sensor::A, true),
                
                // we've got a ball
                incrementBallsState(),
                setMotorState(MotorState::MoveTowardsShooter),
                waitForSensorState(Sensor::B, true),

                // ball collected, now collect another
                gotoState(waitForBall),

                { done, setMotorState(MotorState::Stopped) },
            });
        }
    }
}
