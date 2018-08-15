#include "LifterSetDutyCycleAction.h"

namespace xero {
    namespace phoenix {
        LifterSetDutyCycleAction::LifterSetDutyCycleAction(Lifter& lifter, double value) : LifterAction(lifter) {
            value_ = value;
        }

        void LifterSetDutyCycleAction::start() {
            for (auto motor : getLifter().motors_) {
                getLifter().setMotorsDutyCycle(value_);
            }
        }
        void LifterSetDutyCycleAction::run() {

        }
        bool LifterSetDutyCycleAction::isDone() {
            return true;

        }
        void LifterSetDutyCycleAction::cancel() {

        }
        std::string LifterSetDutyCycleAction::toString() {
            return action_name_;
        }
        const std::string LifterSetDutyCycleAction::action_name_("SetLifterDutyCycle");
    }
}