#include "Intake.h"
#include "IntakeAction.h"
#include <Robot.h>

using namespace xero::base;

namespace xero {
    namespace phoenix {
        Intake::Intake(Robot & robot):Subsystem(robot,"intake") {
           int m1 = robot.getSettingsParser().getInteger("hw:intake:leftmotor") ;
           int m2 = robot.getSettingsParser().getInteger("hw:intake:rightmotor") ;

           motor1_ =std::make_shared<frc::VictorSP>(m1);
           motor2_ =std::make_shared<frc::VictorSP>(m2);
        }

        Intake::~Intake(){
        }

        bool Intake::canAcceptAction(ActionPtr action) {
            auto coll = std::dynamic_pointer_cast<IntakeAction>(action) ;
            return coll != nullptr ;
        }
    }
}
