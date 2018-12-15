#pragma once

#include <singlemotorsubsystem/SingleMotorSubsystem.h>
#include <DigitalInput.h>

namespace xero {
    namespace bunny2018 {
		
        class Intake : public xero::base::SingleMotorSubsystem {

            /// The action to set the motor power is derived from SingleMotorSubsystem
            
        public:
            
            Intake(xero::base::Robot& robot);
            virtual ~Intake(); 
            virtual void computeState();
            virtual bool canAcceptAction(xero::base::ActionPtr action);

        private:

        private:
            std::shared_ptr<frc::DigitalInput> ball_sensor_;
        };
    }
}
