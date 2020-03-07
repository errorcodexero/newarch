#include "ConveyorEmitAction.h"
#include "Conveyor.h"
#include "Droid.h"
#include <gamepiecemanipulator/GamePieceManipulator.h>
#include <gamepiecemanipulator/shooter/Shooter.h>

using namespace xero::misc;

namespace xero {
    namespace droid {
        void ConveyorEmitAction::start() {
            ConveyorStateAction::start();
            shouldStopFiring_ = false;
        }

        ConveyorEmitAction::ConveyorEmitAction(Conveyor &subsystem):
            ConveyorStateAction(subsystem, "ConveyorEmitAction") {
            
            const std::string loop = "loop";
            const std::string done = "done";
            const std::string notFiringLastBall = "firing and loading next ball";
            setStates({
                // if empty, stop
                { loop, branchState(done, [=] { return getSubsystem().isEmpty(); }) },
                
                assertState([=] { return getSubsystem().isStagedForFire(); },
                            "ConveyorEmitAction called with no ball in position; "
                            "was ConveyorPrepareToEmitAction run?"
                ),

                setMotorState(MotorState::MoveTowardsShooter),
                { [=] { 
                    setStagedForCollect(false); 
                    setCollecting(false);
                    return StateResult::Next; 
                }},
                { "wait for current ball to move out of sensor", waitForSensorState(Sensor::D, false) },

                branchState(notFiringLastBall, [=] { return getSubsystem().getBallCount() != 1; }),
                
                // ----------------------------
                // We're firing the last ball.
                { "delay for last ball to clear shooter", delayState(0.5) },
                decrementBallsState(),
                { [=] { setStagedForFire(false); return StateResult::Next; } },
                { "delay in case we picked up extra balls", delayState(1.5) },
                gotoState(done),
                // ----------------------------

                // We're not firing the last ball.
                // Wait for the next ball to get into position.
                { notFiringLastBall, waitForSensorState(Sensor::D, true) },
                { "waiting for next ball to move out of sensor", waitForSensorState(Sensor::D, false) },
                
                decrementBallsState(),

                // if we've been asked to stop firing, we're done
                branchState(done, [=] { return shouldStopFiring_; }),

                gotoState(loop),

                // we're done
                { done, setMotorState(MotorState::Stopped) },
            });
        }

        void ConveyorEmitAction::conveyorActionStarted()
        {
        }

        void ConveyorEmitAction::conveyorActionRun()
        {
        }

        void ConveyorEmitAction::conveyorActionFinished()
        {
        }
    }
} 