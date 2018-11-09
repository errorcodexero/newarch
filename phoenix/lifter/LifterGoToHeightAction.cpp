#include "LifterGoToHeightAction.h"
#include <cmath>
#include "Robot.h"

namespace xero {
    namespace phoenix {
        LifterGoToHeightAction::LifterGoToHeightAction(Lifter& lifter, double target_height) : LifterAction(lifter) {
            target_height_ = target_height;
            PID_controller_.initFromSettingsExtended(getLifter().getRobot().getSettingsParser(), "lifter:goto");			
        }

        LifterGoToHeightAction::LifterGoToHeightAction(Lifter& lifter, const std::string &name) : LifterAction(lifter) {
            target_height_ = getLifter().getRobot().getSettingsParser().getDouble(name) ;
            PID_controller_.initFromSettingsExtended(getLifter().getRobot().getSettingsParser(), "lifter:goto");			
        }		

        void LifterGoToHeightAction::start() {
            PID_controller_.initFromSettingsExtended(getLifter().getRobot().getSettingsParser(), "lifter:goto");
            getLifter().setBrake(false);
        }

        void LifterGoToHeightAction::run() {
            current_height_ = getLifter().getCurrentHeight();
            if (!isDone()) {
                double PID_output = PID_controller_.getOutput(target_height_, current_height_, 0.0, getLifter().getRobot().getDeltaTime());
                getLifter().setMotorsDutyCycle(PID_output);
            }
        }

        bool LifterGoToHeightAction::isDone() {
            double d_error = target_height_ - current_height_;
            double current_v = getLifter().getCurrentVelocity();
            double d_threshold = getLifter().getRobot().getSettingsParser().getDouble("lifter:goto:distance_threshold");
            double v_threshold = getLifter().getRobot().getSettingsParser().getDouble("lifter:goto:speed_threshold");
            if (std::fabs(d_error) < d_threshold && (std::fabs(current_v) < v_threshold)) {
                getLifter().setMotorsDutyCycle(0.0);
                getLifter().setBrake(true);
                return true;
            }
            else return false;
        }

        void LifterGoToHeightAction::cancel() {

        }

        std::string LifterGoToHeightAction::toString() {
			std::string ret("GoToHeight ") ;
			ret += std::to_string(target_height_) ;
            return ret ;
        }
    }
}