#pragma once

#include <motorencodersubsystem/MotorEncoderSubsystem.h>
#include <motors/MotorController.h> 

namespace xero {
    namespace droid {
        class IntakeAction ;

        class Intake : public xero::base::MotorEncoderSubsystem {
        public:
            Intake(xero::base::Subsystem *parent);
            virtual ~Intake() ;

            double current_power_ = 0.0 ;
            std::shared_ptr<xero::base::MotorController> collector_ ;

            bool canAcceptAction(xero::base::ActionPtr action) ;
        };
    }
}
