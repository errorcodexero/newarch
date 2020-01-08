#include <motorencodersubsystem/MotorEncoderVelocityAction.h>

namespace xero {
    namespace droid {
        class ShooterFireAction : public MotorEncoderVelocityAction {
            ShooterFireAction(Shooter &shooter): shooter_(shooter) {}

            void run() override {
                // TODO: Compute target velocity
                double targetVel = 0;

                setTarget(targetVel);
                MotorEncoderVelocityAction::run();
            }
        private:
            Shooter &shooter_;
            
        }
    }
}