#include "DumpHatch.h"
#include "CarlosHatch.h"
#include "CarlosHatchArmAction.h"
#include <Robot.h>

using namespace xero::misc ;

namespace xero {
    namespace phaser {
        DumpHatch::DumpHatch(CarlosHatch &subsystem) : CarlosHatchAction(subsystem) {
        }

        DumpHatch::~DumpHatch() {
        }

        void DumpHatch::start() {
            CarlosHatch &hatchholder = getSubsystem() ;
            if (hatchholder.isArmDeployed()) {
                hatchholder.disableHooks() ;
                state_ = State::CloseFingers ;
            }
            else {
                hatchholder.extendArm() ;
                state_ = State::ExtendArm ;
            }
            start_ = getSubsystem().getRobot().getTime() ;            
        }

        void DumpHatch::run() {
            switch(state_) {
            case State::ExtendArm:
                if (getSubsystem().getRobot().getTime() - start_ > 0.5) {
                    state_ = State::CloseFingers ;
                    getSubsystem().disableHooks() ;
                    start_ = getSubsystem().getRobot().getTime() ;
                }
                break ;

            case State::CloseFingers:
                if (getSubsystem().getRobot().getTime() - start_ > 0.25) {
                    getSubsystem().retractArm() ;
                    state_ = State::RetractArm ;
                    start_ = getSubsystem().getRobot().getTime() ;
                }
                break ;

            case State::RetractArm:
                if (getSubsystem().getRobot().getTime() - start_ > 0.1) {
                    state_ = State::Idle ;
                }            
                break ;      

            case State::Idle:
                break ;      
            }
        }

        bool DumpHatch::isDone() {
            return state_ == State::Idle ;
        }

        void DumpHatch::cancel() {
            state_  = State::Idle ;
        }

        std::string DumpHatch::toString() {
            std::string result = "DumpHatch " ;
            return result ;
        }
    }
}
