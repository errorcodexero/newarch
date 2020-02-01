
#include "ClimbAction.h"
#include "Climber.h"
#include "motorencodersubsystem/MotorEncoderGoToAction.h"

using namespace xero::base ;

namespace xero {
    namespace droid {
        void ClimbAction::start() {   
        }
        void ClimbAction::run() {
            getClimber().getLifter()->setAction(std::make_shared<MotorEncoderGoToAction>(*getClimber().getLifter(), "climber:motor:climb")) ;
            getClimber().getServoR()->Set(-1.0) ;               //("climber:servo:r:climb") ;
            getClimber().getServoL()->Set(-1.0) ;               //("climber:servo:l:climb") ;   
        }
        std::string ClimbAction::toString() {
            return "ClimbAction" ;
        }

    }
}
