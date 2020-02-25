#pragma once

#include "gamepiecemanipulator/shooter/Shooter.h"
#include <motorencodersubsystem/MotorEncoderVelocityAction.h>
#include <networktables/NetworkTableEntry.h>
#include <frc/shuffleboard/Shuffleboard.h>

namespace xero {
    namespace droid {
        class Shooter ;

        class ShooterVelocityAction : public xero::base::MotorEncoderVelocityAction {

        public:

            ShooterVelocityAction(Shooter &sub, double target, Shooter::HoodPosition pos) ;
            virtual ~ShooterVelocityAction() ;

             Shooter &getSubsystem() {
                return subsystem_ ;
            }
     
            virtual void run() override ;

            virtual void cancel() override ;

            virtual void setTarget(double target) override;

            virtual std::string toString() ;

            Shooter::HoodPosition  getHood() { return hood_; }
            void setHood(Shooter::HoodPosition  hood) { hood_ = hood; }

        private:

            Shooter &subsystem_;
            double ready_margin_percent_;

            Shooter::HoodPosition hood_;

            void updateReadyToFire();
        };
    }
}