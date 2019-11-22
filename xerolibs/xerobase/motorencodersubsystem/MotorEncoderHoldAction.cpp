#include "MotorEncoderHoldAction.h"

#include "Robot.h"

namespace xero {
    namespace base {
        MotorEncoderHoldAction::MotorEncoderHoldAction(MotorEncoderSubsystem &subsystem, double target):
            MotorEncoderSubsystemAction(subsystem), target_(target), hasExplicitTarget_(true) {
                pid_.initFromSettingsExtended(
                    subsystem.getRobot().getSettingsParser(),
                    subsystem.configName_ + ":hold"
                );
        }

        void MotorEncoderHoldAction::start() {
            if (!hasExplicitTarget_) target_ = getSubsystem().getPosition();
            cancelled_ = false;
            pid_.reset();
        }

        void MotorEncoderHoldAction::run() {
            auto &sub = getSubsystem();

            double distance = normalizePosition(target_ - sub.getPosition());
            sub.setMotor(pid_.getOutput(0, distance, sub.getRobot().getDeltaTime()));
        }
    }
}