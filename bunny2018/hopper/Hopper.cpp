#include "Hopper.h"
#include "singlemotorsubsystem/SingleMotorSubsystemAction.h"
#include <Robot.h>

using namespace xero::base;

namespace xero {
    namespace bunny2018 {
        Hopper::Hopper(Robot & robot):Subsystem(robot,"hopper") {
           int m = robot.getSettingsParser().getInteger("hw:hopper:motor") ;

           motor_ =std::make_shared<frc::VictorSP>(m);
        }

        Hopper::~Hopper(){
        }

        bool Hopper::canAcceptAction(ActionPtr action) {
            auto coll = std::dynamic_pointer_cast<SingleMotorSubsystemAction>(action) ;
            return coll != nullptr ;
        }
    }
}
