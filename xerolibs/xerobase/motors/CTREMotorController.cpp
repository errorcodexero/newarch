#include <algorithm>
#include <cassert>

#include "CTREMotorController.h"

namespace xero {
    namespace base {
            CTREMotorController::CTREMotorController(int id, Type type) {
                switch (type) {
                case Type::TalonSRX:
                    motor_ = std::make_shared<ctre::phoenix::motorcontrol::can::TalonSRX>(id); 
                    break;
                case Type::TalonFX:
                    {
                        SupplyCurrentLimitConfiguration limit(true, 30, 30, 5) ;
                        auto m = std::make_shared<ctre::phoenix::motorcontrol::can::TalonFX>(id);
                        m->ConfigSupplyCurrentLimit(limit) ;
                        m->GetSensorCollection().SetIntegratedSensorPosition(0.0) ;
                        m->SetStatusFramePeriod(Status_2_Feedback0_, 10) ;
                        motor_ = m ;
                    }
                    break;
                case Type::VictorSPX:
                    motor_ = std::make_shared<ctre::phoenix::motorcontrol::can::VictorSPX>(id);
                    break;
                    } 

                motor_->ConfigVoltageCompSaturation(12.0, 10) ;
                motor_->EnableVoltageCompensation(true) ;

                type_ = type ;
            }

            void CTREMotorController::setCurrentLimit(double curlim)
            {
                auto m = std::dynamic_pointer_cast<ctre::phoenix::motorcontrol::can::TalonFX>(motor_) ;
                SupplyCurrentLimitConfiguration limit(true, curlim, curlim, 1) ;
                m->ConfigSupplyCurrentLimit(limit) ;
            }

            void CTREMotorController::follow(std::shared_ptr<MotorController> motor, bool invert) {
                if (auto m = std::dynamic_pointer_cast<CTREMotorController>(motor)) {
                    motor_->Follow(*m->motor_);
                    if (invert) motor_->SetInverted(ctre::phoenix::motorcontrol::InvertType::OpposeMaster);
                    else motor_->SetInverted(ctre::phoenix::motorcontrol::InvertType::FollowMaster);
                } else assert(0 == "CTRE motors can only follow other CTRE motors");
            }

            void CTREMotorController::set(double percent) {
                motor_->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, percent);
            }

            void CTREMotorController::setInverted(bool inverted) {
                isInverted_ = inverted;
                motor_->SetInverted(inverted);
            }

            void CTREMotorController::reapplyInverted() { setInverted(isInverted_); }

            void CTREMotorController::setNeutralMode(NeutralMode neutralMode) {
                ctre::phoenix::motorcontrol::NeutralMode mode;
                if (neutralMode == NeutralMode::Coast) mode = ctre::phoenix::motorcontrol::NeutralMode::Coast;
                else mode = ctre::phoenix::motorcontrol::NeutralMode::Brake;

                motor_->SetNeutralMode(mode);
            }
    }
}
