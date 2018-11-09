#pragma once

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

            virtual bool canAcceptAction(ActionPtr action) ;

        private:
            std::shared_ptr<frc::VictorSP> motor_;

        }  ;
    }
}