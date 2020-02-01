
#include "Climber.h"
#include "ReleaseAction.h"
#include <motorencodersubsystem/MotorEncoderGoToAction.h>

using namespace xero::base ;

namespace xero {
    namespace droid {
        
        void ReleaseAction::start() {
        }   

        void ReleaseAction::run() {
            getClimber().getLifter()->setAction(std::make_shared<MotorEncoderGoToAction>(*getClimber().getLifter(), "climber:motor:release")) ;
            getClimber().getServoR()->Set(1.0) ;               //("climber:servo:r:release") ;
            getClimber().getServoL()->Set(1.0) ;               //("climber:servo:l:release") ;
        }

        std::string ReleaseAction::toString() {     
            return "ReleaseAction" ;
        }

    }
}

//open the 2 servos (run forward)
//release with 2 motors (run backward)

//single motor + add a servo
//servo : hitec hs-422
