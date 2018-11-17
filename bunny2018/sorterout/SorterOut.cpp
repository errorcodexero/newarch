#include "SorterOut.h"
#include "singlemotorsubsystem/SingleMotorSubsystemAction.h"
#include <Robot.h>

using namespace xero::base;

namespace xero {
    namespace bunny2018 {
        SorterOut::SorterOut(Robot & robot):Subsystem(robot,"sorter_out") {
           int m = robot.getSettingsParser().getInteger("hw:sorter_out:motor") ;

           motor_ =std::make_shared<frc::VictorSP>(m);
        }

        SorterOut::~SorterOut(){
        }

        bool SorterOut::canAcceptAction(ActionPtr action) {
            auto coll = std::dynamic_pointer_cast<SingleMotorSubsystemAction>(action) ;
            return coll != nullptr ;
        }
    }
}
