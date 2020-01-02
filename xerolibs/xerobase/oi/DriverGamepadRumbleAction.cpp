#include "DriverGamepadRumbleAction.h"
#include "Robot.h"

using namespace xero::misc ;

namespace xero {
    namespace base {

        DriverGamepadRumbleAction::DriverGamepadRumbleAction(OISubsystem &oi, bool left, double value, double duration) : GenericAction(oi.getRobot().getMessageLogger()), oi_(oi) {
            value_ = value ;
            left_ = left ;
            duration_ = duration ;
        }

        void DriverGamepadRumbleAction::start() {
            auto driver = oi_.getDriverGamepad() ;
            driver->rumble(left_, value_, duration_) ;
            setDone();
        }

        void DriverGamepadRumbleAction::run() {
        }

        void DriverGamepadRumbleAction::cancel() {
        }

        std::string DriverGamepadRumbleAction::toString() {
            std::string result ;

            result = "DriverGamepadRumbleAction" ;
            if (left_)
                result += " left" ;
            else
                result += " right" ;
            
            result += " value " + std::to_string(value_) ;
            result += " duration " + std::to_string(duration_) ;
            
            return result ;
        }
    }
}
