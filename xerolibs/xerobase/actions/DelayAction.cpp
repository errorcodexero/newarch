#include "actions/DelayAction.h"

namespace xero {
    namespace base {

        DelayAction::DelayAction(double delay) {
            delayTime_ = delay ;
        }

        void DelayAction::start() {
            startTime_ = frc::Timer::GetFPGATimestamp();
            isDone_ = false;
        }

        void DelayAction::run() {
        }

        bool DelayAction::isDone() {
            double elapsed = frc::Timer::GetFPGATimestamp()-startTime_;
            if (elapsed >= delayTime_) {
                isDone_ = true;
            }
            return isDone_;
        }

        void DelayAction::cancel() {
            isDone_ = true;
        }

        std::string DelayAction::toString() {
            std::string result ;

            result += "DelayAction " ;
            result += std::to_string(delayTime_) ;
            return result ;
        }
    }
}
