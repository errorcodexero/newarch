#include "SparkMaxMotorController.h"
#include <algorithm>
#include <cassert>

namespace xero {
    namespace base {
            SparkMaxMotorController::SparkMaxMotorController(int id, bool brushless) {
                rev::CANSparkMaxLowLevel::MotorType type;
                if (brushless) type = rev::CANSparkMaxLowLevel::MotorType::kBrushless;
                else type = rev::CANSparkMaxLowLevel::MotorType::kBrushed;

                motor_ = std::make_shared<rev::CANSparkMax>(id, type);
                motor_->SetSmartCurrentLimit(30) ;

                // TODO: is this correct? any other setup?
                motor_->EnableVoltageCompensation(12.0) ;
            }

            void SparkMaxMotorController::follow(std::shared_ptr<MotorController> motor, bool invert) {
                if (auto m = std::dynamic_pointer_cast<SparkMaxMotorController>(motor)) {
                    motor_->Follow(*m->motor_, invert);
                } else assert(0 == "SparkMax motors can only follow other SparkMax motors");
            }

            void SparkMaxMotorController::set(double percent) {
                motor_->Set(percent);
            }

            void SparkMaxMotorController::setInverted(bool inverted) {
                isInverted_ = inverted;
                motor_->SetInverted(inverted);
            }

            void SparkMaxMotorController::reapplyInverted() { setInverted(isInverted_); }

            void SparkMaxMotorController::setNeutralMode(NeutralMode neutralMode) {
                rev::CANSparkMax::IdleMode mode;
                if (neutralMode == NeutralMode::Coast) mode = rev::CANSparkMax::IdleMode::kCoast;
                else mode = rev::CANSparkMax::IdleMode::kBrake;

                motor_->SetIdleMode(mode);
            }
    }
}
