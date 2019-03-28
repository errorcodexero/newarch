#include "CarlosHatchStartAction.h"
#include "CarlosHatch.h"
#include <Robot.h>

using namespace xero::misc ;


namespace xero {
    namespace phaser {
        CarlosHatchStartAction::CarlosHatchStartAction(CarlosHatch &subsystem, bool push) : CarlosHatchAction(subsystem) {
            push_arm_ = push ;
        }

        CarlosHatchStartAction::~CarlosHatchStartAction() {
        }

        void CarlosHatchStartAction::start() {
            CarlosHatch &hatchholder = getSubsystem() ;
            if (!hatchholder.hasHatch())
                hatchholder.disableHooks() ;

            hatchholder.extendArm() ;
            waiting_ = true ;
        }

        void CarlosHatchStartAction::run() {
            CarlosHatch &hatchholder = getSubsystem() ;
            if (waiting_) {
                if (!hatchholder.isImpacting()) {
                    if (!push_arm_)
                        hatchholder.stopArm() ;                    
                    waiting_ = false ;
                }
            }
        }

        bool CarlosHatchStartAction::isDone() {
            return !waiting_ ;
        }

        void CarlosHatchStartAction::cancel() {
            waiting_ = false ;
        }

        std::string CarlosHatchStartAction::toString() {
            std::string result = "CarlosHatchStartAction " ;
            return result ;
        }
    }
}
