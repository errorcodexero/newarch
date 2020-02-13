
#include "Climber.h"
#include "ReleaseAction.h"
#include <motorencodersubsystem/MotorEncoderGoToAction.h>

using namespace xero::base ;

namespace xero {
    namespace droid {
        
        void ReleaseAction::start() {
        }   

        void ReleaseAction::run() {
            double r_release_ = getClimber().getRobot().getSettingsParser().getDouble("climber:servo:r:release") ;
            double l_release_ = getClimber().getRobot().getSettingsParser().getDouble("climber:servo:l:release") ;

            getClimber().getLifter()->setAction(std::make_shared<MotorEncoderGoToAction>(*getClimber().getLifter(), "climber:motor:follower:down")) ;
            // climber motor(s) = trapezoidal speed profile w/ velocity and acceleration in .dat file
            getClimber().getServoR()->Set(r_release_) ;
            getClimber().getServoL()->Set(l_release_) ;
        }
        std::string ReleaseAction::toString() {     
            return "ReleaseAction" ;
        }

    }
}
