#pragma once

#include <motorencodersubsystem/MotorEncoderVelocityAction.h>
#include <networktables/NetworkTableEntry.h>
#include <frc/shuffleboard/Shuffleboard.h>

namespace xero {
    namespace droid {
        class Shooter ;

        class ShooterVelocityAction : public xero::base::MotorEncoderVelocityAction {

        public:

            ShooterVelocityAction(Shooter &sub, double rpm) ;
            virtual ~ShooterVelocityAction() ;

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