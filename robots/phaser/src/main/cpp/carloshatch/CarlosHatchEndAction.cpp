#include "CarlosHatchEndAction.h"
#include "CarlosHatch.h"
#include <Robot.h>

using namespace xero::misc ;

namespace xero {
    namespace phaser {
        CarlosHatchEndAction::CarlosHatchEndAction(CarlosHatch &subsystem) : CarlosHatchAction(subsystem) {
        }

        CarlosHatchEndAction::~CarlosHatchEndAction() {
        }

        void CarlosHatchEndAction::start() {
            CarlosHatch &hatchholder = getSubsystem() ;
            state_ = State::WaitForImpact ;
            collecting_ = !hatchholder.hasHatch() ;
        }

        void CarlosHatchEndAction::run() {
            CarlosHatch &hatchholder = getSubsystem() ;

            switch(state_) {
            case State::WaitForImpact:
                if (hatchholder.isImpacting()) {
                    if (collecting_)
                        hatchholder.enableHooks() ;
                    else
                        hatchholder.disableHooks() ;
                        
                    start_ = hatchholder.getRobot().getTime() ;
                    state_ = State::WaitForHooks ;
                }
                break ;

            case State::WaitForHooks:
                if (hatchholder.getRobot().getTime() - start_ > 0.1) {
                    hatchholder.retractArm() ;
                    state_ = State::Idle ;
                }
                break ;

            case State::Idle:
                break ;
            }
        }

        bool CarlosHatchEndAction::isDone() {
            return state_ == State::Idle ;
        }

        void CarlosHatchEndAction::cancel() {
            state_  = State::Idle ;
        }

        std::string CarlosHatchEndAction::toString() {
            std::string result = "CarlosHatchEndAction " ;
            return result ;
        }
    }
}
