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
                { waitForBall, setMotorState(MotorState::Stopped) },
                // if full, stop
                branchState(done, [=] { return getSubsystem().isFull(); } ),

                //assertState([=]() { return getSubsystem().isEmpty() 
                //                        || getSubsystem().readSensor(Sensor::B);
                //            },  "ConveyorReceiveAction called with balls out of position; "
                //                "was ConveyorPrepareToEmitAction run?"
                //),
                
                { "wait for collect start sensor", waitForSensorState(Sensor::A, true) },
                
                //{ "delay for ball to enter belt", delayState(0.01) },
                
                // we've got a ball
                incrementBallsState(),

                // Pick a collect sequence
                branchState(secondThroughFifth, [=]{ return getSubsystem().getBallCount() != 1; }),

                // 1st
                { first, setMotorState(MotorState::MoveTowardsShooter) },

                // wait for rising edge then falling edge on shooter sensor
                { "1st-ball: wait for ball to reach shooter (rising)", waitForSensorState(Sensor::D, true) },
                { "1st-ball: wait for ball to reach shooter (falling)", waitForSensorState(Sensor::D, false) },

                // ball collected, now collect another
                gotoState(waitForBall),

                // 2nd-5th
                { secondThroughFifth, setMotorState(MotorState::Collect2nd5th) },

                { "wait for old ball to move out of collect finish sensor", waitForSensorState(Sensor::B, false) },

                // if we're full, stop here (we don't have enough space to move the ball all the way)
                branchState(done, [=] { return getSubsystem().isFull(); } ),

                { "wait for ball to move into collect finish sensor", waitForSensorState(Sensor::B, true) },

                // ball collected, now collect another
                gotoState(waitForBall),

                { done, setMotorState(MotorState::Stopped) },
            });
        }
    }
}
