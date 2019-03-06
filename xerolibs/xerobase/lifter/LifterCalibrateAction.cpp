#include "LifterCalibrateAction.h"
#include "Lifter.h"
#include <Robot.h>

namespace xero {
    namespace base {
        LifterCalibrateAction::LifterCalibrateAction(Lifter &lifter, double height, double power, size_t samples) : LifterAction(lifter) {
            power_ = power ;
            samples_ = samples ;
            height_ = height ;
        }

        LifterCalibrateAction::~LifterCalibrateAction() {            
        }

        std::string LifterCalibrateAction::toString() {
            std::string result = "LifteRToBottom " + std::to_string(power_) ;
            return result ;
        }

        void LifterCalibrateAction::start() {
            is_done_ = false ;
            getLifter().setMotorPower(power_) ;
        }

        void LifterCalibrateAction::run() {
            counts_.push_back(getLifter().getEncoderValue()) ;
            if (counts_.size() > samples_)
                counts_.pop_front() ;

            if (std::abs(counts_.front() - counts_.back()) < 2) {
                getLifter().setHeight(height_) ;
                is_done_ = true ;
            }
        }
    }
}
