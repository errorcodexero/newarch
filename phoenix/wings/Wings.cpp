#include "Wings.h"
#include "WingsAction.h"
#include <Robot.h>

namespace xero {
    namespace phoenix {
        Wings::Wings(xero::base::Robot &robot) : xero::base::Subsystem(robot, "wings") {
            int sol = robot.getParams().getValue("hw:wings:solenoid").getInteger() ;
            solenoid_ = std::make_shared<frc::Solenoid>(sol) ;
        }

        Wings::~Wings() {

        }

        void Wings::computeState() {            
        }

        bool Wings::canAcceptAction(xero::base::ActionPtr action) {
            std::shared_ptr<WingsAction> action_p = std::dynamic_pointer_cast<WingsAction>(action) ;
            return action_p != nullptr ;
        }

        void Wings::run() {
            Subsystem::run() ;
            
            auto action_p = std::dynamic_pointer_cast<WingsAction>(getAction()) ;
            if (action_p != nullptr && action_p->getDeploy())
                solenoid_->Set(true) ;
        }
    }
}