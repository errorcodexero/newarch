#include "TubArm.h"
#include "TubArmAction.h"
#include <Robot.h>
#include <actions/SequenceAction.h>
#include <frc/DigitalInput.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace ranseur {
      
        TubArm::TubArm(Subsystem *parent) : Subsystem(parent, "tubarm") {
    
        }

        TubArm::~TubArm() {
        }

        // sees whether or not 
        bool TubArm::canAcceptAction(ActionPtr action) {
            auto tub_arm_act_p = std::dynamic_pointer_cast<TubArmAction>(action) ;
            if (tub_arm_act_p == nullptr)
                return false ;

            return true ;
        }

        void TubArm::computeState() {
            Subsystem::computeState() ;
        }        
    }
}
