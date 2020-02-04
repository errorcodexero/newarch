#pragma once

#include <motorencodersubsystem/MotorEncoderVelocityAction.h>
#include <networktables/NetworkTableEntry.h>
#include <frc/shuffleboard/Shuffleboard.h>

namespace xero {
    namespace droid {
        class Shooter ;

        class FireAction : public xero::base::MotorEncoderVelocityAction {
        public:
            FireAction(Shooter &sub) ;
            virtual ~FireAction() ;

            virtual void run() override ;

        private:
            Shooter &sub_;
            frc::SimpleWidget widget_ ;          
        };
    }
}