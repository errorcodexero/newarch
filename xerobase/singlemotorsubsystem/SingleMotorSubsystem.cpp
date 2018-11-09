#include "SingleMotorSubsystem.h"
#include "Robot.h"

using namespace xero::misc;

namespace xero {
    namespace base {
        SingleMotorSubsystem::SingleMotorSubsystem(Robot & robot):SingleMotorSubsystem(robot,"single_motor_subsystem") {
           int m = robot.getSettingsParser().getInteger("hw:single_motor_subsystem:motor") ;

           motor_ =std::make_shared<frc::VictorSP>(m);
        }

        SingleMotorSubsystem::~SingleMotorSubsystem(){
        }

        bool SingleMotorSubsystem::canAcceptAction(ActionPtr action) {
            auto coll = std::dynamic_pointer_cast<SingleMotorSubsystemAction>(action) ;
            return coll != nullptr ;
        }
    }
}