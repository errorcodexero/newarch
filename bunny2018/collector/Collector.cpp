#include "Collector.h"
#include "singlemotorsubsystem/SingleMotorSubsystemAction.h"
#include <Robot.h>

using namespace xero::base;

namespace xero {
    namespace bunny2018 {
        Collector::Collector(Robot & robot):Subsystem(robot,"collector") {
           int m = robot.getSettingsParser().getInteger("hw:collector:motor") ;

           motor_ =std::make_shared<frc::VictorSP>(m);
        }

        Collector::~Collector(){
        }

        bool Collector::canAcceptAction(ActionPtr action) {
            auto coll = std::dynamic_pointer_cast<SingleMotorSubsystemAction>(action) ;
            return coll != nullptr ;
        }
    }
}
