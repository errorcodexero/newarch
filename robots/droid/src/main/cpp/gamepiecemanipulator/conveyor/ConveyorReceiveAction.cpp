#include "ConveyorReceiveAction.h"
#include "Conveyor.h"

namespace xero {
    namespace droid {
        ConveyorReceiveAction::ConveyorReceiveAction(Conveyor &subsystem):
            ConveyorStateAction(subsystem, "ConveyorReceiveAction") {
            
            const std::string waitForBall = "loop";
            const std::string done = "done";
            const std::string first = "1st collect sequence";
            const std::string secondThroughFifth = "2nd-5th collect sequence";
            const std::string fifth = "5th collect sequence";
            setStates({
                { waitForBall, [=]{ collecting_ = false; return StateResult::Next; } },
                // if full, stop
                branchState(done, [=] { return getSubsystem().isFull(); } ),

                assertState([=]() { return getSubsystem().isStagedForCollect(); },
                                "ConveyorReceiveAction called with balls out of position; "
                                "was ConveyorPrepareToEmitAction run?"
                ),
                
                { "wait for collect start sensor", waitForSensorState(Sensor::A, true) },
                
                //{ "delay for ball to enter belt", delayState(0.01) },
                
                // we've got a ball
                incrementBallsState(),
                // run the motors to collect
                { [=]{ collecting_ = true; return StateResult::Next; } },

                { "wait for old ball to move out of collect finish sensor", waitForSensorState(Sensor::B, false) },

                // if we're full, stop here (we don't have enough space to move the ball all the way)
                branchState(done, [=] { return getSubsystem().isFull(); } ),

                { "wait for ball to move into collect finish sensor", waitForSensorState(Sensor::B, true) },

                // ball collected, now collect another
                gotoState(waitForBall),

                { done, setMotorState(MotorState::Stopped) },
            });
        }

        void ConveyorReceiveAction::conveyorActionStarted() {
            reachedShooterSensor_ = false;
            collecting_ = false;
            setMotors(MotorState::Stopped);
        }

        void ConveyorReceiveAction::conveyorActionRun() {
            // Once a ball passes the shooter sensor,
            // it's in position to fire
            if (!reachedShooterSensor_) {
                if (getSubsystem().readSensor(Sensor::D))
                    reachedShooterSensor_ = true;
            } else if (!getSubsystem().readSensor(Sensor::D)) {
                reachedShooterSensor_ = false;
                setStagedForFire(true);
            }

            if (collecting_) {
                if (getSubsystem().isStagedForFire()) {
                    // If a ball is in position to fire, don't
                    // run it any further
                    setMotors(MotorState::MoveCollectMotorOnly);
                } else {
                    setMotors(MotorState::MoveTowardsShooter);
                }
            } else {
                setMotors(MotorState::Stopped);
            }
        }
    }
}
