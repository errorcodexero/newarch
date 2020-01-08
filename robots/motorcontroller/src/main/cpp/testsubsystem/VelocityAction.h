#include <motorencodersubsystem/MotorEncoderVelocityAction.h>

namespace xero {
    namespace motorcontroller {
        class VelocityAction : public MotorEncoderVelocityAction {
            VelocityAction(TestSubsystem &sub): MotorEncoderVelocityAction(sub, 0), sub_(sub) {}

            void run() override {
                setTarget(frc::SmartDashboard::GetNumber("velocity", getTarget()));
                MotorEncoderVelocityAction::run();
            }
        private:
            TestSubsystem &sub_;
            
        }
    }
}