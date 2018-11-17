#pragma once

#include "Subsystem.h" 
#include "SingleMotorSubsystemAction.h" 
#include <VictorSP.h>
#include <memory>

namespace xero {
    namespace base {

        class SingleMotorSubsystem : public Subsystem {
			friend class SingleMotorSubsystemAction;
        public:

            SingleMotorSubsystem(Robot &robot);

            virtual ~SingleMotorSubsystem();

            std::shared_ptr<frc::VictorSP> &getMotor()
            {
                return motor_;
            }

            virtual bool canAcceptAction(ActionPtr action) ;

        private:
            std::shared_ptr<frc::VictorSP> motor_;

        }  ;
    }
}