#include "ranseurids.h"
#include "TubManipulatorSubsystem.h"
#include <Robot.h>
#include <list>
#include "tubcollector/Collector.h"
#include "tubarm/TubArm.h"
#include "tubwrist/TubWrist.h"
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

            tubarm_ = std::make_shared<TubArm>(this) ;
            addChild(tubarm_) ;            
          
            tubwrist_ = std::make_shared<TubWrist>(this) ;
            addChild(tubwrist_) ;            
        
        }

        bool TubManipulatorSubsystem::canAcceptAction(ActionPtr act) {
            return false;
        }

        std::string TubManipulatorSubsystem::toString() {
            return " " ;
        }

    }
}
