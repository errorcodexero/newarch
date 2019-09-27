#include <algorithm>
#include "CTREMotorController.h"

namespace xero {
    namespace base {
            CTREMotorController::CTREMotorController(int id, Type type) {
                switch (type) {
                case Type::TalonSRX:
                    motor_ = std::make_shared<ctre::phoenix::motorcontrol::can::TalonSRX>(id); 
                    break;
                case Type::VictorSPX:
                    motor_ = std::make_shared<ctre::phoenix::motorcontrol::can::VictorSPX>(id);
                    break;
                    } 

                motor_->ConfigVoltageCompSaturation(12.0, 10) ;
                motor_->EnableVoltageCompensation(true) ;
            }

            void CTREMotorController::set(double percent) {
                motor_->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, percent);
            }

            void CTREMotorController::setInverted(bool inverted) {
                motor_->SetInverted(true);
            }

            void CTREMotorController::setNeutralMode(NeutralMode neutralMode) {
                ctre::phoenix::motorcontrol::NeutralMode mode;
                if (neutralMode == NeutralMode::Coast) mode = ctre::phoenix::motorcontrol::NeutralMode::Coast;
                else mode = ctre::phoenix::motorcontrol::NeutralMode::Brake;

                motor_->SetNeutralMode(mode);
            }
    }
}
