#pragma once

#include <motorencodersubsystem/MotorEncoderVelocityAction.h>
#include <networktables/NetworkTableEntry.h>
#include <frc/shuffleboard/Shuffleboard.h>

namespace xero {
    namespace droid {
        class Shooter ;

        class ShooterVelocityAction : public xero::base::MotorEncoderVelocityAction {

        public:

            ShooterVelocityAction(Shooter &sub, double target) ;
            virtual ~ShooterVelocityAction() ;

             Shooter &getSubsystem() {
                return subsystem_ ;
            }
     
            virtual void run() override ;

            virtual void cancel() override ;

            virtual void setTarget(double target) override;

        private:

            Shooter &subsystem_;
            double ready_margin_percent_;

            void updateReadyToFire();
        };
    }
}