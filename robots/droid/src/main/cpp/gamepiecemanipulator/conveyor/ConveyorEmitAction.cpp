#include "ConveyorEmitAction.h"
#include "Conveyor.h"

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
            setStates({
                // if empty, stop
                branchState(done, [=] { return getSubsystem().isEmpty(); }),

                // TODO: find a better way to tell if balls are staged
                //assertState([=] { return getSubsystem().readSensor(Sensor::D); },
                //            "ConveyorEmitAction called with no ball in position; "
                //            "was ConveyorPrepareToEmitAction run?"
                //),
                
                // move a ball towards the shooter
                { loop, setMotorState(MotorState::MoveTowardsShooter) },
                { "wait for ball to clear sensor", waitForSensorState(Sensor::D, false) },

                { "delay for ball to fire", delayState(0.5) },
                
                decrementBallsState(),
                // if empty, stop
                branchState(done, [=] { return getSubsystem().isEmpty(); }),

                // wait for the next ball to move into position
                { "wait for next ball to reach sensor", waitForSensorState(Sensor::D, true) },

                // if we've been asked to stop firing, we're done
                branchState(done, [=] { return shouldStopFiring_; }),

                gotoState(loop),

                // we're done
                { done, setMotorState(MotorState::Stopped) },
            });
        }
    }
} 