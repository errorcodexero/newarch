#include <algorithm>
#include "SparkMaxMotorController.h"

namespace xero {
    namespace base {
            SparkMaxMotorController::SparkMaxMotorController(int id, bool brushless) {
                rev::CANSparkMaxLowLevel::MotorType type;
                if (brushless) type = rev::CANSparkMaxLowLevel::MotorType::kBrushless;
                else type = rev::CANSparkMaxLowLevel::MotorType::kBrushed;

                motor_ = std::make_shared<rev::CANSparkMax>(id, type);

                // TODO: is this correct? any other setup?
                motor_->EnableVoltageCompensation(12.0) ;
            }

            void SparkMaxMotorController::set(double percent) {
                motor_->Set(percent);
            }

            void SparkMaxMotorController::setInverted(bool inverted) {
                motor_->SetInverted(true);
            }

            void SparkMaxMotorController::setNeutralMode(NeutralMode neutralMode) {
                rev::CANSparkMax::IdleMode mode;
                if (neutralMode == NeutralMode::Coast) mode = rev::CANSparkMax::IdleMode::kCoast;
                else mode = rev::CANSparkMax::IdleMode::kBrake;

                motor_->SetIdleMode(mode);
            }
    }
}