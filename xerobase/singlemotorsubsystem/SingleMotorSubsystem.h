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
        public:
            SingleMotorSubsystem(Robot &robot, const std::string &name, const std::string &motor, bool victor=false) ;
            SingleMotorSubsystem(Robot &robot, const std::string &name, int motor, bool victor=false) ;

            virtual ~SingleMotorSubsystem();

            void setMotor(double v)
            {
				if (vmotor_)
					vmotor_->Set(v) ;
				else
					tmotor_->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, v) ;
            }

            virtual bool canAcceptAction(ActionPtr action) ;

        private:
            std::shared_ptr<frc::VictorSP> vmotor_;
            std::shared_ptr<TalonSRX> tmotor_;			
        }  ;
    }
}
