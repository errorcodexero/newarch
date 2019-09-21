#include <algorithm>
#include "SparkMaxMotorController.h"

namespace xero {
    namespace base {
            SparkMaxMotorController::SparkMaxMotorController(const std::list<int> &ids, bool brushless) {
                rev::CANSparkMaxLowLevel::MotorType type;
                if (brushless) type = rev::CANSparkMaxLowLevel::MotorType::kBrushless;
                else type = rev::CANSparkMaxLowLevel::MotorType::kBrushed;

                std::transform(ids.begin(), ids.end(), std::back_inserter(motors_), [type](int id) -> MotorPtr {
                    MotorPtr motor = std::make_shared<rev::CANSparkMax>(id, type);

                    // TODO: is this correct? any other setup?
                    motor->EnableVoltageCompensation(12.0) ;

                    return motor;
                });
                
                // Set all motors (except the first) to follow the first
                MotorPtr first = nullptr;
                for (MotorPtr motor : motors_) {
                    if (first) motor->Follow(*first);
                    else first = motor;
                }
            }

            void SparkMaxMotorController::set(double percent) {
                motors_.front()->Set(percent);
            }

            void SparkMaxMotorController::setInverted(bool inverted) {
                for (MotorPtr motor : motors_) motor->SetInverted(true);
            }

            void SparkMaxMotorController::setNeutralMode(NeutralMode neutralMode) {
                rev::CANSparkMax::IdleMode mode;
                if (neutralMode == NeutralMode::Coast) mode = rev::CANSparkMax::IdleMode::kCoast;
                else mode = rev::CANSparkMax::IdleMode::kBrake;

                for (MotorPtr motor : motors_) motor->SetIdleMode(mode);
            }
    }
}