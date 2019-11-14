#include "ranseurids.h"
#include "TubManipulatorSubsystem.h"
#include <Robot.h>
#include <list>
#include "tubcollector/TubCollector.h"
#include "tubarm/TubArm.h"
#include "tubwrist/TubWrist.h"
#include <actions/Action.h>
#include "Subsystem.h"


using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace ranseur {
        TubManipulatorSubsystem::TubManipulatorSubsystem(xero::base::Subsystem* parent) : Subsystem(parent, "tubmanipulator") {

            tubcollector_ = std::make_shared<TubCollector>(this) ;
            addChild(tubcollector_) ;        

            tubarm_ = std::make_shared<TubArm>(this) ;
            addChild(tubarm_) ;            
          
            tubwrist_ = std::make_shared<TubWrist>(this) ;
            addChild(tubwrist_) ;            
        
        }

        bool TubManipulatorSubsystem::canAcceptAction(ActionPtr action) {
            return true;
        }

        std::string TubManipulatorSubsystem::toString() {
            return " " ;
        }

    }
}
