#include "CarlosHatchImpactAction.h"
#include "CarlosHatch.h"
#include <Robot.h>
#include <oi/DriverGamepadRumbleAction.h>
#include <oi/OISubsystem.h>

using namespace xero::misc ;


namespace xero {
    namespace phaser {
        CarlosHatchImpactAction::CarlosHatchImpactAction(CarlosHatch &subsystem, bool push) : CarlosHatchAction(subsystem) {
            auto oi = subsystem.getRobot().getOI() ;
            push_arm_ = push ;
            wait_for_hooks_time_ = subsystem.getRobot().getSettingsParser().getDouble("carloshatch:waitforhooks") ;
            rumble_ = std::make_shared<DriverGamepadRumbleAction>(*oi, true, 1.0, 1.0) ;
        }

        CarlosHatchImpactAction::~CarlosHatchImpactAction() {
        }

        void CarlosHatchImpactAction::start() {
            CarlosHatch &hatchholder = getSubsystem() ;
            if (!hatchholder.hasHatch())
                hatchholder.disableHooks() ;

            collecting_ = !hatchholder.hasHatch() ;                

            hatchholder.extendArm() ;
            state_ = State::WaitForFullyExtended ;
        }

        void CarlosHatchImpactAction::run() {
            CarlosHatch &hatchholder = getSubsystem() ;

            switch(state_) {
            case State::WaitForFullyExtended:
                if (hatchholder.isFullyExtended()) {
                    state_ = State::WaitForImpact ;
                    if (!push_arm_)
                        hatchholder.stopArm() ;
                }
                break ;

            case State::WaitForImpact:
                if (!hatchholder.isFullyExtended()) {
                    if (collecting_)
                        hatchholder.enableHooks() ;
                    else
                        hatchholder.disableHooks() ;
                        
                    start_ = hatchholder.getRobot().getTime() ;
                    state_ = State::WaitForHooks ;
                }
                break ;

            case State::WaitForHooks:
                if (hatchholder.getRobot().getTime() - start_ > wait_for_hooks_time_) {
                    hatchholder.retractArm() ;
                    hatchholder.getRobot().getOI()->setAction(rumble_) ;
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
