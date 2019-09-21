#include <algorithm>
#include "CTREMotorController.h"

namespace xero {
    namespace base {
            CTREMotorController::CTREMotorController(const std::list<int> &ids, Type type) {
                std::transform(ids.begin(), ids.end(), std::back_inserter(motors_), [type](int id) -> MotorPtr {
                    MotorPtr motor;
                    switch (type) {
                    case Type::TalonSRX:
                        motor = std::make_shared<ctre::phoenix::motorcontrol::can::TalonSRX>(id);
                        break;
                    case Type::VictorSPX:
                        motor = std::make_shared<ctre::phoenix::motorcontrol::can::VictorSPX>(id);
                        break;
                    } 

                    motor->ConfigVoltageCompSaturation(12.0, 10) ;
                    motor->EnableVoltageCompensation(true) ;
                    return motor;
                });
                
                // Set all motors (except the first) to follow the first
                MotorPtr first = nullptr;
                for (MotorPtr motor : motors_) {
                    if (first) motor->Follow(*first);
                    else first = motor;
                }
            }

            void CTREMotorController::set(double percent) {
                motors_.front()->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, percent);
            }

            void CTREMotorController::setInverted(bool inverted) {
                for (MotorPtr motor : motors_) motor->SetInverted(true);
            }

            void CTREMotorController::setNeutralMode(NeutralMode neutralMode) {
                ctre::phoenix::motorcontrol::NeutralMode mode;
                if (neutralMode == NeutralMode::Coast) mode = ctre::phoenix::motorcontrol::NeutralMode::Coast;
                else mode = ctre::phoenix::motorcontrol::NeutralMode::Brake;

                for (MotorPtr motor : motors_) motor->SetNeutralMode(mode);
            }
    }
}
