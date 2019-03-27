#include "CarlosHatchImpactAction.h"
#include "CarlosHatch.h"
#include <Robot.h>

using namespace xero::misc ;


namespace xero {
    namespace phaser {
        CarlosHatchImpactAction::CarlosHatchImpactAction(CarlosHatch &subsystem) : CarlosHatchAction(subsystem) {
            duration_ = subsystem.getRobot().getSettingsParser().getDouble("carloshatch:impact:duration") ;
        }

        CarlosHatchImpactAction::~CarlosHatchImpactAction() {
        }

        void CarlosHatchImpactAction::start() {
            timing_ = false ;
        }

        void CarlosHatchImpactAction::run() {
            CarlosHatch &hatchholder = getSubsystem() ;
            if (hatchholder.isImpacting()) {
                if (hatchholder.isHolderDeployed())
                    hatchholder.deactivateHolder() ;
                else
                    hatchholder.activateHolder() ;

                start_ = getSubsystem().getRobot().getTime() ;
                timing_ = true ;
            }

            if (timing_ && hatchholder.getRobot().getTime() - start_ > duration_)
                is_done_ = true ;
        }

        bool CarlosHatchImpactAction::isDone() {
            return is_done_ ;
        }

        void CarlosHatchImpactAction::cancel() {
            is_done_ = true ;
        }

        std::string CarlosHatchImpactAction::toString() {
            std::string result = "CarlosHatchImpactAction " ;
            return result ;
        }
    }
}
