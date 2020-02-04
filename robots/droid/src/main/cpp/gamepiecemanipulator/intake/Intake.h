#pragma once

#include <motorencodersubsystem/MotorEncoderSubsystem.h>
#include <motors/MotorController.h> 

namespace xero {
    namespace droid {
        class Intake : public xero::base::MotorEncoderSubsystem {
        public:
            Intake(xero::base::Subsystem *parent);
            virtual ~Intake() {}

            double current_power_ = 0.0 ;
            std::shared_ptr<xero::base::MotorController> collector_ ;
  
        };
    }
}
