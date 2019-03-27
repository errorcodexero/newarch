#include "DriverGamepadRumbleAction.h"

using namespace xero::misc ;

namespace xero {
    namespace base {

        DriverGamepadRumbleAction::DriverGamepadRumbleAction(OISubsystem &oi, bool left, int count, double value, double duration) : oi_(oi) {
            value_ = value ;
            left_ = left ;
            duration_ = duration ;
            count_ = count ;
        }

        void DriverGamepadRumbleAction::start() {
            auto driver = oi_.getDriverGamepad() ;
            driver->rumble(left_, count_, value_, duration_) ;
        }

        void DriverGamepadRumbleAction::run() {
        }

        bool DriverGamepadRumbleAction::isDone() {
            return true ;
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
