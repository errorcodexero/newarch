#pragma once

#include "Subsystem.h" 
#include "SingleMotorSubsystemAction.h" 
#include "XeroTalonSRX.h"

typedef ctre::phoenix::motorcontrol::can::TalonSRX TalonSRX;

namespace xero {
    namespace base {

        class SingleMotorSubsystem : public Subsystem {
			friend class SingleMotorSubsystemAction;
        public:
            SingleMotorSubsystem(Robot &robot, const std::string &name, const std::string &motor);
            SingleMotorSubsystem(Robot &robot, const std::string &name, int motor);

            virtual ~SingleMotorSubsystem();

            std::shared_ptr<TalonSRX> &getMotor()
            {
                return motor_;
            }

            virtual bool canAcceptAction(ActionPtr action) ;

        private:
            std::shared_ptr<TalonSRX> motor_;

        }  ;
    }
}