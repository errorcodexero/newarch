#include "ClimbAction.h"
#include "Climber.h"
#include "motorencodersubsystem/MotorEncoderGoToAction.h"
#include "Droid.h"

using namespace xero::base ;

namespace xero {
    namespace droid {
        ClimbAction::ClimbAction(Climber &climber) : ClimberAction(climber){
        }

        void ClimbAction::start() {   
        }

        void ClimbAction::run() {
            double r_climb_ = getClimber().getRobot().getSettingsParser().getDouble("climber:servo:r:climb") ;
            double l_climb_ = getClimber().getRobot().getSettingsParser().getDouble("climber:servo:l:climb") ;

            getClimber().getLifter()->setAction(std::make_shared<MotorEncoderGoToAction>(*getClimber().getLifter(), "climber:motor:follower:up")) ;
            // climber motor(s) = trapezoidal speed profile w/ velocity and acceleration in .dat file
            getClimber().getServoR()->Set(r_climb_) ; 
            getClimber().getServoL()->Set(l_climb_) ; 
        }

        std::string ClimbAction::toString() {
            return "ClimbAction" ;
        }

    }
}
