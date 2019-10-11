#include "BunnyArm.h"
#include "BunnyArmAction.h"
#include <Robot.h>

using namespace xero::base ;

namespace xero {
    namespace ranseur {
        BunnyArm::BunnyArm(xero::base::Robot &robot) : xero::base::Subsystem(robot, "bunnyarm") {
            int sol = robot.getSettingsParser().getInteger("hw:bunnyarm:solenoid") ;
            // added "hw:bunnyarm:solenoid" to phaser.dat as 6

            solenoid_ = std::make_shared<frc::Solenoid>(sol) ;
            solenoid_->Set(false) ;

            deployed_ = false ;
        }

        BunnyArm::~BunnyArm() {
        }

        void BunnyArm::computeState() {
            if (solenoid_->Get())
                deployed_ = true ;
            else
                deployed_ = false ;
        }

        bool BunnyArm::canAcceptAction(xero::base::ActionPtr action) {
            std::shared_ptr<BunnyArmAction> action_p = std::dynamic_pointer_cast<BunnyArmAction>(action) ;
            return action_p != nullptr ;
        }
    }
}
