#include "LiftingCollectorSetDutyCycleAction.h"

namespace xero {
    namespace phoenix {
        LiftingCollectorSetDutyCycleAction::LiftingCollectorSetDutyCycleAction(LiftingCollector& lifter, double value) : LiftingCollectorAction(lifter) {
            value_ = value;
        }

        void LiftingCollectorSetDutyCycleAction::start() {
            for (auto motor : getLiftingCollector().motors_) {
                getLiftingCollector().setMotorsDutyCycle(value_);
            }
        }
        void LiftingCollectorSetDutyCycleAction::run() {

        }
        bool LiftingCollectorSetDutyCycleAction::isDone() {
            return true;

        }
        void LiftingCollectorSetDutyCycleAction::cancel() {

        }
        std::string LiftingCollectorSetDutyCycleAction::toString() {
            return action_name_;
        }
        const std::string LiftingCollectorSetDutyCycleAction::action_name_("SetLiftingCollectorDutyCycle");
    }
}