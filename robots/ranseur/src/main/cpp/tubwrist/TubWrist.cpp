#include "TubWrist.h"
#include "TubWristAction.h"
#include "ranseurids.h"
#include <Robot.h>
#include <actions/SequenceAction.h>
#include <frc/DigitalInput.h>
#include <motorencodersubsystem/MotorEncoderSubsystem.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace ranseur {
      
    TubWrist::TubWrist(Subsystem *parent) : MotorEncoderSubsystem(parent, "tubwrist", "hw:tubwrist", MSG_GROUP_TUBWRIST, true) {
     
        }

        TubWrist::~TubWrist() {
        }

        // sees whether or not 
       bool TubWrist::canAcceptAction(ActionPtr action) {
            if (MotorEncoderSubsystem::canAcceptAction(action))
                return true ;

            auto tub_wrist_act_p = std::dynamic_pointer_cast<TubWristAction>(action) ;
            return (tub_wrist_act_p != nullptr);
        }

        void TubWrist::computeState() {
            MotorEncoderSubsystem::computeState() ;
        }        
    }
}
