#include "Intake.h"
#include "IntakeAction.h"
#include <Robot.h>

using namespace xero::base;

namespace xero {
    namespace ranseur {
        
        Intake::Intake(Robot &robot): Subsystem(robot,"intakeWheels") {
        robot.getMotorFactory() -> createMotor("intakeWheels");
     
        }

        Intake::~Intake(){
        }

        bool Intake::canAcceptAction(ActionPtr action) {
            auto coll = std::dynamic_pointer_cast<IntakeAction>(action) ;
            return coll != nullptr ;
        }
    }
}