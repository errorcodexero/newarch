#include "ranseurids.h"
#include "TubManipulatorSubsystem.h"
#include <Robot.h>
#include <list>
#include "tubmanipulator/tubcollector/Collector.h"
#include "TubManipulatorSubsystem.h"
#include <actions/Action.h>

//using namespace xero::ranseur::Collector ;
using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace ranseur {
        TubManipulatorSubsystem::TubManipulatorSubsystem(Robot &robot) : RobotSubsystem(robot, "ranseur") {

            collector_ = std::make_shared<Collector>(this) ;
            addChild(collector_) ;                    
        }

        bool TubManipulatorSubsystem::canAcceptAction(ActionPtr act) {
            return false;
        }

        std::string TubManipulatorSubsystem::toString() {
            return " " ;
        }

    }
}
