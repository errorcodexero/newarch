#include "LifterCalibrateAction.h"

const double BOTTOM_HEIGHT = 11.375;

namespace xero {
    namespace phoenix {
        void LifterCalibrateAction::start() {

        }
        void LifterCalibrateAction::run() {
            getLifter().encoder_->Reset();
            getLifter().encoder_ticks_ = 0;
            getLifter().height_ = BOTTOM_HEIGHT;
        }
        bool LifterCalibrateAction::isDone() {
            return true;

        }
        void LifterCalibrateAction::cancel() {

        }
        std::string LifterCalibrateAction::toString() {
            return action_name_;
        }
        const std::string LifterCalibrateAction::action_name_("CalibrateLifter");
    }
}