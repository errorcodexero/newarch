#include "SparkMaxMotorController.h"
#include <algorithm>
#include <cassert>

namespace xero {
    namespace base {
            static SparkMaxMotorController::MotorPtr createMotor(int id, bool brushless) {
                rev::CANSparkMaxLowLevel::MotorType type;
                if (brushless) type = rev::CANSparkMaxLowLevel::MotorType::kBrushless;
                else type = rev::CANSparkMaxLowLevel::MotorType::kBrushed;

                return std::make_shared<rev::CANSparkMax>(id, type);
            }

            SparkMaxMotorController::SparkMaxMotorController(int id, bool brushless): 
                motor_(createMotor(id, brushless)), encoder_(motor_->GetEncoder())
            {
                motor_->RestoreFactoryDefaults(false) ;
                motor_->SetSmartCurrentLimit(30) ;
                motor_->EnableVoltageCompensation(12.0) ;
                motor_->BurnFlash() ;

                resetPosition();
            }

            void SparkMaxMotorController::setCurrentLimit(double curlim)
            {
                motor_->SetSmartCurrentLimit(curlim) ;                
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
