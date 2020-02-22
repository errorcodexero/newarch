#include "ExtendClimberAction.h"
#include "Climber.h"
#include "motorencodersubsystem/MotorEncoderGoToAction.h"
#include "Droid.h"

using namespace xero::base ;

namespace xero {
    namespace droid {
        ExtendClimberAction::ExtendClimberAction(Climber &climber) : ClimberAction(climber) {
            servo_right_ = getSubsystem().getRobot().getSettingsParser().getDouble("climber:servo:r:climb") ;
            servo_left_ = getSubsystem().getRobot().getSettingsParser().getDouble("climber:servo:l:climb") ;
            servo_delay_ = getSubsystem().getRobot().getSettingsParser().getDouble("climber:servo:delay") ;
            release_action_ = std::make_shared<MotorEncoderGoToAction>(*climber.getLifter(), "climber:climb_height") ;
        }

        void ExtendClimberAction::start() {   
            state_ = State::ReleasingServos ;
            start_ = getSubsystem().getRobot().getTime() ;
            getSubsystem().setLeftServo(servo_left_) ;
            getSubsystem().setRightServo(servo_right_) ;
        }

        void ExtendClimberAction::run() {

            switch (state_)
            {
                case State::ReleasingServos:
                    if (getSubsystem().getRobot().getTime() - start_ > servo_delay_)
                    {
                       getSubsystem().getLifter()->setAction(release_action_) ;
                       state_ = State::ExtendingClimber ;
                    }
                    break ;

                case State::ExtendingClimber:
                    if (getSubsystem().getLifter()->isDone())
                    {
                        state_ = State::Done ;
                        setDone() ;
                    }
                    break ;

                case State::Done:
                    break ;
            }
        }

        std::string ExtendClimberAction::toString() {
            return "ExtendClimberAction" ;
        }
    }
}
