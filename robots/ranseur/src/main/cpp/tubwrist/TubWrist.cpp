#include "TubWrist.h"
#include "TubWrist.h"
#include <Robot.h>
#include <actions/SequenceAction.h>
#include <frc/DigitalInput.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace ranseur {
      
        TubWrist::TubWrist(Subsystem *parent) : Subsystem(parent, "tubwrist") {
    
        }

        TubWrist::~TubWrist() {
        }

        // sees whether or not 
        bool TubWrist::canAcceptAction(ActionPtr action) {
            auto tub_wrist_act_p = std::dynamic_pointer_cast<TubWrist>(action) ;
            if (tub_wrist_act_p == nullptr)
                return false ;

            return true ;
        }

        void TubWrist::computeState() {
            Subsystem::computeState() ;
        }        
    }
}
