#include "actions/DelayAction.h"

namespace xero {
    namespace base {

        DelayAction::DelayAction(xero::misc::MessageLogger &logger, double delay) : GenericAction(logger) {
            delayTime_ = delay ;
        }

        void DelayAction::start() {
            Action::start();
            startTime_ = frc::Timer::GetFPGATimestamp();
        }

        void DelayAction::run() {
            Action::run();
            double elapsed = frc::Timer::GetFPGATimestamp()-startTime_;
            if (elapsed >= delayTime_)
                setDone();
        }

        void DelayAction::cancel() {
            Action::cancel();
            setDone();
        }

        std::string DelayAction::toString() {
            std::string result ;

            result += "DelayAction " ;
            result += std::to_string(delayTime_) ;
            return result ;
        }
    }
}
