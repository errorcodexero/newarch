#include "BunnyArm.h"
#include "BunnyArmAction.h"
#include <Robot.h>

using namespace xero::base ;

namespace xero {
    namespace ranseur {
        BunnyArm::BunnyArm(Subsystem *parent) : xero::base::Subsystem(parent, "bunnyarm") {
            auto &robot = getRobot();

            int sol = robot.getSettingsParser().getInteger("hw:bunnyarm:solenoid") ;
            solenoid_ = std::make_shared<frc::Solenoid>(sol) ;
            solenoid_->Set(false) ;
            raised_ = true ;
        }

        BunnyArm::~BunnyArm() {
        }

        void BunnyArm::computeMyState() {
        }

        bool BunnyArm::canAcceptAction(xero::base::ActionPtr action) {
            std::shared_ptr<BunnyArmAction> action_p = std::dynamic_pointer_cast<BunnyArmAction>(action) ;
            return action_p != nullptr ;
        }
    }
}
