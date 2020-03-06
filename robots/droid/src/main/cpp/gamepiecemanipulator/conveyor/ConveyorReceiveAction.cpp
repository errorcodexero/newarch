#include "ConveyorReceiveAction.h"
#include "Conveyor.h"
#include "droidids.h"
#include <MessageLogger.h>

using namespace xero::base;
using namespace xero::misc;
namespace xero {
    namespace droid {
        ConveyorReceiveAction::ConveyorReceiveAction(Conveyor &subsystem):
            ConveyorStateAction(subsystem, "ConveyorReceiveAction") {

            auto &settings = subsystem.getRobot().getSettingsParser();
            double timeoutDuration = settings.getDouble("conveyor:receive:timeout");
            //double prepareReceiveTimeoutDuration = settings.getDouble("conveyor:prepare_receive:timeout");
            
            const std::string waitForBall = "loop";
            const std::string done = "done";
            const std::string first = "1st collect sequence";
            const std::string secondThroughFifth = "2nd-5th collect sequence";
            const std::string fifth = "5th collect sequence";
            const std::string timeout = "collect timeout";
            //const std::string timeout2 = "timeout while handling timeout";
            setStates({
                { waitForBall, [=]{ collecting_ = false; return StateResult::Next; } },
                { [=]() { setCollecting(false); return StateResult::Next; }},
                // if full, stop
                branchState(done, [=] { return getSubsystem().isFull(); } ),

                assertState([=]() { return getSubsystem().isStagedForCollect(); },
                                "ConveyorReceiveAction called with balls out of position; "
                                "was ConveyorPrepareToReceiveAction run?"
                ),
                
                { "wait for collect start sensor", waitForSensorState(Sensor::A, true) },

                { [=]() { setCollecting(true); return StateResult::Next; }},
                
                { "delay for ball to enter belt", delayState(0.03) },

                // run the motors to collect
                { [=]{ collecting_ = true; return StateResult::Next; } },

                // if we're full, use a different sensor (we don't have enough space to move the ball all the way)
                branchState(fifth, [=] { return getSubsystem().getBallCount() == 4; } ),

                { "wait for old ball to move out of collect finish sensor", waitForSensorState(Sensor::B, false) },

                { "wait for ball to move into collect finish sensor", 
                    waitForSensorState(Sensor::B, true, timeout, timeoutDuration) },

                // we've got a ball
                incrementBallsState(),                

                { "delay before collecting next ball", delayState(0.01) },

                // ball collected, now collect another
                gotoState(waitForBall),

                // if we're full, wait for the last ball to break sensor C
                { fifth, waitForSensorState(Sensor::C, false) },
                waitForSensorState(Sensor::C, true, timeout, timeoutDuration),

                // we've got the fifth ball; we're done.
                { [=]() { setCollecting(false); return StateResult::Next; }},

                // Run motors backwards for a bit to clear up jams
                { [=]() { finishing_ = true; return StateResult::Next; }},

                delayState(0.2),

                incrementBallsState(),                      

                gotoState(done),


                // Timeout handling
                { timeout, [=]() { setCollecting(false); return StateResult::Next; }},

                // If empty, just try again
                gotoState(waitForBall),
                //{ branchState(waitForBall, [=]{ return getSubsystem().isEmpty(); }) },

                // Run motors backwards
                /*{ [=]() { 
                    if (getSubsystem().isStagedForFire())
                        setMotors(MotorState::ReverseCollectMotorOnly);
                    else
                        setMotors(MotorState::MoveTowardsIntake);
                    return StateResult::Next;
                } },
                
                { "timeout: wait for balls to reach end", 
                    waitForSensorState(Sensor::A, true, timeout2, prepareReceiveTimeoutDuration) },
                
                { [=]() { 
                    if (getSubsystem().isStagedForFire())
                        setMotors(MotorState::MoveCollectMotorOnly);
                    else
                        setMotors(MotorState::ReverseCollectMotorOnly);
                    return StateResult::Next;
                } },
                { "timeout: wait for balls to clear end sensor", waitForSensorState(Sensor::A, false) },
                { "timeout: wait for balls to reach target sensor", 
                    waitForSensorState(Sensor::B, true, timeout, timeoutDuration) },

                // Try collecting again.
                gotoState(waitForBall),

                
                // Recursive timeout handling
                // If we timed out while handling a timeout, the conveyor is probably empty.
                { timeout2, [=]{
                    setStagedForCollect(true);
                    setStagedForFire(false);
                    setBallCount(0);
                    return StateResult::Next;
                }},
                // Try collecting again.
                gotoState(waitForBall),*/
                { done, [=] { finishing_ = false; return StateResult::Next; } },
                setMotorState(MotorState::Stopped)
            });
        }

        void ConveyorReceiveAction::conveyorActionStarted() {
            reachedShooterSensor_ = false;
            collecting_ = false;
            finishing_ = false;
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
            } else if (finishing_) {
                auto &logger = getSubsystem().getRobot().getMessageLogger();
                logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_CONVEYOR);
                logger << "ConveyorReceiveAction: collected 5 balls, running backwards to un-jam";
                logger.endMessage();
                setMotors(MotorState::MoveTowardsIntake);
            } else {
                setMotors(MotorState::Stopped);
            }
        }
    }
}
