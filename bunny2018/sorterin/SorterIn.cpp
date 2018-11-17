#include "SorterIn.h"
#include "singlemotorsubsystem/SingleMotorSubsystemAction.h"
#include <Robot.h>

using namespace xero::base;

namespace xero {
    namespace bunny2018 {
        SorterIn::SorterIn(Robot & robot):Subsystem(robot,"sorter_in") {
           int m = robot.getSettingsParser().getInteger("hw:sorter_in:motor") ;

           motor_ =std::make_shared<frc::VictorSP>(m);
        }

        SorterIn::~SorterIn(){
        }

        bool SorterIn::canAcceptAction(ActionPtr action) {
            auto coll = std::dynamic_pointer_cast<SingleMotorSubsystemAction>(action) ;
            return coll != nullptr ;
        }
    }
}
