#pragma once

#include <motorencodersubsystem/MotorEncoderVelocityAction.h>
#include <networktables/NetworkTableEntry.h>
#include <frc/shuffleboard/Shuffleboard.h>

namespace xero {
    namespace droid {
        class Shooter ;

        class FireAction : public xero::base::MotorEncoderVelocityAction {

        public:

            FireAction(Shooter &sub, double target) ;
            virtual ~FireAction() ;

             Shooter &getSubsystem() {
                return subsystem_ ;
            }
     
            virtual void run() override ;

            virtual void cancel() override ;

        private:
            Shooter &subsystem_;
            double target_;
            double ready_margin_percent_;
        };
    }
}