#pragma once

#include "Subsystem.h" 
#include "SingleMotorSubsystemAction.h" 
#include "XeroTalonSRX.h"
#include <VictorSP.h>

typedef ctre::phoenix::motorcontrol::can::TalonSRX TalonSRX;

namespace xero {
    namespace base {

        class SingleMotorSubsystem : public Subsystem {
			friend class SingleMotorSubsystemAction;
		private:
			constexpr static double epsilon = 1e-3 ;

        public:
            SingleMotorSubsystem(Robot &robot, const std::string &name, const std::string &motor, bool victor=false) ;
            SingleMotorSubsystem(Robot &robot, const std::string &name, int motor, bool victor=false) ;

            virtual ~SingleMotorSubsystem();

            void setMotor(double v)
            {
				if (vmotor_ != nullptr)
					vmotor_->Set(v) ;
				else
					tmotor_->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, v) ;

				current_power_ = 0.0 ;
            }

            virtual bool canAcceptAction(ActionPtr action) ;

			bool isRunning() const {
				return current_power_ > epsilon ;
			}

        private:
            std::shared_ptr<frc::VictorSP> vmotor_;
            std::shared_ptr<TalonSRX> tmotor_;
			double current_power_ ;
        }  ;
    }
}
