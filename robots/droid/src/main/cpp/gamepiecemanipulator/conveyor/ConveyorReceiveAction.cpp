#include "ConveyorReceiveAction.h"
#include "Conveyor.h"

// Sensor names used in comments:
//                    (shooter)
//                      |   |
//                      |   < C
//                      |   |
//          *-----------/   |
// (intake) *              /
//          *-^---^------ /
//            A   B

namespace xero {
    namespace droid {
        // void ConveyorReceiveAction::start() {
        //     if (/* is full */false) {
        //         setDone();
        //     }
        //     state_ = State::Preparing;
        // }

        // void ConveyorReceiveAction::run() {
        //     State oldState;
        //     Conveyor &conveyor = getSubsystem();
        //     do {
        //         oldState = state_;
        //         switch (state_) {
        //         case State::Preparing:
        //             conveyor.setMotor(-1);    // move balls toward intake
        //             if (/* sensor B is tripped */true) {
        //                 // balls are in position at back of intake
        //                 state_ = State::Ready;
        //             }
        //             break;
        //         case State::Ready:
        //             conveyor.setMotor(0); // wait to receive
        //             if (/* sensor A is tripped */true) {
        //                 state_ = State::StartingReceive;
        //             }
        //             break;
        //         case State::StartingReceive:
        //             // A new ball is moving through sensor A. Wait for the previous
        //             // ball to clear sensor B so we can detect when the new ball gets there.
        //             conveyor.setMotor(1); // move balls toward shooter
        //             if (/* sensor B is clear*/true) {
        //                 state_ = State::FinishingReceive;
        //             }
        //             break;
        //         case State::FinishingReceive:
        //             // We're receiving a ball, but it has not yet reached sensor B.
        //             conveyor.setMotor(1); // move balls toward shooter
        //             if (/* sensor B is tripped */true) {
        //                 conveyor.ballCount_++;
        //                 // we're done once the conveyor is full
        //                 if (conveyor.ballCount_ == Conveyor::MAX_BALLS) setDone();
        //                 else state_ = State::Ready;
        //             }
        //             break;
        //         }
        //     } while (state_ != oldState);   // if we changed state, we can run again
        // }
    }
}