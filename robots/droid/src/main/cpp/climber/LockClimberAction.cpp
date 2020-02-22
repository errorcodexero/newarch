#include "LockClimberAction.h"
#include "Climber.h"
#include "Droid.h"

using namespace xero::base ;

namespace xero {
    namespace droid {
        LockClimberAction::LockClimberAction(Climber &climber) : ClimberAction(climber) {
            servo_right_ = getSubsystem().getRobot().getSettingsParser().getDouble("climber:servo:r:climb") ;
            servo_left_ = getSubsystem().getRobot().getSettingsParser().getDouble("climber:servo:l:climb") ;
            servo_delay_ = getSubsystem().getRobot().getSettingsParser().getDouble("climber:servo:delay") ;
        }

        LockClimberAction::~LockClimberAction()
        {
        }

        void LockClimberAction::start() 
        {   
            start_ = getSubsystem().getRobot().getTime() ;
            getSubsystem().setLeftServo(servo_left_) ;
            getSubsystem().setRightServo(servo_right_) ;
        }

        void LockClimberAction::run() 
        {
            if (getSubsystem().getRobot().getTime() - start_ > servo_delay_)
            {
                setDone() ;
            }
        }

        std::string LockClimberAction::toString() {
            return "LockClimberAction" ;
        }
    }
}
