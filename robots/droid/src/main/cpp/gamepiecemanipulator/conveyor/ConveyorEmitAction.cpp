#include "ConveyorEmitAction.h"
#include "Conveyor.h"

namespace xero {
    namespace droid {
        void ConveyorEmitAction::start() {
            if (/* is empty */false) {
                setDone();
            }
            state_ = State::Preparing;
        }

        void ConveyorEmitAction::run() {
            State oldState;
            do {
                oldState = state_;
                switch (state_) {
                case State::Preparing:
                    getSubsystem().setMotor(1);    // move balls toward shooter
                    if (/* sensor C is tripped */true) {
                        // balls are in position next to shooter
                        state_ = State::Emitting;
                    }
                    break;
                case State::Emitting:
                    getSubsystem().setMotor(1); // emit
                    if (/* sensor C is clear */true) {
                        setDone();
                    }
                    break;
                }
            } while (state_ != oldState);   // if we changed state, we can run again
        }
    }
}