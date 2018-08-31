#include "LiftingCollectorGoToHeightAction.h"
#include <cmath>
#include "Robot.h"

namespace xero {
    namespace phoenix {
        LiftingCollectorGoToHeightAction::LiftingCollectorGoToHeightAction(LiftingCollector& lifter, double target_height) : LiftingCollectorAction(lifter) {
            target_height_ = target_height;
        }

        void LiftingCollectorGoToHeightAction::start() {
            PID_controller_.initFromSettingsExtended(getLiftingCollector().getRobot().getSettingsParser(), "lifter:goto");
            getLiftingCollector().setBrake(false);
        }

        void LiftingCollectorGoToHeightAction::run() {
            current_height_ = getLiftingCollector().getCurrentHeight();
            if (!isDone()) {
                double PID_output = PID_controller_.getOutput(target_height_, current_height_, getLiftingCollector().getRobot().getDeltaTime());
                getLiftingCollector().setMotorsDutyCycle(PID_output);
            }
        }

        bool LiftingCollectorGoToHeightAction::isDone() {
            double d_error = target_height_ - current_height_;
            double current_v = getLiftingCollector().getCurrentVelocity();
            double d_threshold = getLiftingCollector().getRobot().getSettingsParser().getDouble("lifter:goto:distance_threshold");
            double v_threshold = getLiftingCollector().getRobot().getSettingsParser().getDouble("lifter:goto:speed_threshold");
            if (std::fabs(d_error) < d_threshold && (std::fabs(current_v) < v_threshold)) {
                getLiftingCollector().setMotorsDutyCycle(0.0);
                getLiftingCollector().setBrake(true);
                return true;
            }
            else return false;
        }

        void LiftingCollectorGoToHeightAction::cancel() {

        }

        std::string LiftingCollectorGoToHeightAction::toString() {
			std::string ret("GoToHeight ") ;
			ret += std::to_string(target_height_) ;
            return ret ;
        }
    }
}