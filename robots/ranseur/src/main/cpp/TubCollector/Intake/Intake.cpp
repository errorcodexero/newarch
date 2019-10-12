#include "Intake.h"
#include "IntakeAction.h"
#include <Robot.h>

using namespace xero::base;

namespace xero {
    namespace phoenix {
        
        Intake::Intake(Robot &robot, std::string configID): Subsystem(robot,"intake") {
        robot.getMotorFactory() -> createMotor("");
     
        }

        Intake::~Intake(){
        }

        bool Intake::canAcceptAction(ActionPtr action) {
            auto coll = std::dynamic_pointer_cast<IntakeAction>(action) ;
            return coll != nullptr ;
        }
    }
}
