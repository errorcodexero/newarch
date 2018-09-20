#include "LifterChangeGearAction.h"

namespace xero {
    namespace phoenix {
        LifterChangeGearAction::LifterChangeGearAction(Lifter& lifter, Lifter::LIFTER_GEAR value) : LifterAction(lifter) {
            value_ = value;
        }

        void LifterChangeGearAction::start() {
            getLifter().setGear(value_);
        }
        void LifterChangeGearAction::run() {

        }
        bool LifterChangeGearAction::isDone() {
            return true;

        }
        void LifterChangeGearAction::cancel() {

        }
        std::string LifterChangeGearAction::toString() {
            return action_name_;
        }
        const std::string LifterChangeGearAction::action_name_("LifterChangeGear");
    }
}