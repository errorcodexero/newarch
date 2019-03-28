#include "CarlosHatchImpactAction.h"
#include "CarlosHatch.h"
#include <Robot.h>

using namespace xero::misc ;


namespace xero {
    namespace phaser {
        CarlosHatchImpactAction::CarlosHatchImpactAction(CarlosHatch &subsystem, bool push) : CarlosHatchAction(subsystem) {
            push_arm_ = push ;
        }

        CarlosHatchImpactAction::~CarlosHatchImpactAction() {
        }

        void CarlosHatchImpactAction::start() {
            CarlosHatch &hatchholder = getSubsystem() ;
            if (!hatchholder.hasHatch())
                hatchholder.disableHooks() ;

            hatchholder.extendArm() ;
            state_ = State::WaitForSwitch ;

            collecting_ = !hatchholder.hasHatch() ;
        }

        void CarlosHatchImpactAction::run() {
            CarlosHatch &hatchholder = getSubsystem() ;

            switch(state_) {
            case State::WaitForSwitch:
                if (!hatchholder.isImpacting()) {
                    state_ = State::WaitForImpact ;
                    if (!push_arm_)
                        hatchholder.stopArm() ;
                }
                break ;

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

        bool CarlosHatchImpactAction::isDone() {
            return state_ == State::Idle ;
        }

        void CarlosHatchImpactAction::cancel() {
            state_  = State::Idle ;
        }

        std::string CarlosHatchImpactAction::toString() {
            std::string result = "CarlosHatchImpactAction " ;
            return result ;
        }
    }
}
