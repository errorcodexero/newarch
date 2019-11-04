#include "Climber.h"
#include "ClimberAction.h"
#include <Robot.h>

using namespace xero::base ;

namespace xero {
    namespace phaser {
        Climber::Climber(xero::base::Robot &robot) : xero::base::Subsystem(robot, "climber") {
            int sol = robot.getSettingsParser().getInteger("hw:climber:solenoid") ;
            // added "hw:climber:solenoid" to phaser.dat as 6
            // solenoid 1,2,5 are already assigned

            solenoid_ = std::make_shared<frc::Solenoid>(sol) ;
            solenoid_->Set(false) ;

            deployed_ = false ;
        }

        Climber::~Climber() {
        }

        void Climber::computeState() {
            if (solenoid_->Get())
                deployed_ = true ;
            else
                deployed_ = false ;
        }

        bool Climber::canAcceptAction(xero::base::ActionPtr action) {
            std::shared_ptr<ClimberAction> action_p = std::dynamic_pointer_cast<ClimberAction>(action) ;
            return action_p != nullptr ;
        }
    }
}
