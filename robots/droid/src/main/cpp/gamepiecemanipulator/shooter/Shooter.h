#pragma once

#include <motorencodersubsystem/MotorEncoderSubsystem.h>
#include <frc/Servo.h>

namespace xero {
    namespace droid {
        class Turret;

        class Shooter : public xero::base::MotorEncoderSubsystem {
        public:
            enum class HoodPosition
            {
                Up,
                Down,
                Unknown
            } ;        

        public:

            Shooter(xero::base::Subsystem *parent);
            virtual ~Shooter() {}

            void setReadyToFire(bool ready_to_fire) {
                ready_to_fire_ = ready_to_fire;
            }

            bool isReadyToFire() const {
                return ready_to_fire_;
            }

            void setHood(HoodPosition pos);

            virtual void run() ;

            bool canAcceptAction(xero::base::ActionPtr action);

        private:
            void updateHood() ;

        private:
            std::shared_ptr<frc::Servo> hoodServo_;

            double hoodUpPos_;
            double hoodDownPos_;

            HoodPosition desired_ ;
            HoodPosition actual_ ;

            bool ready_to_fire_ ;      // Set to true when shooter at the required shooting velocity
        };
    }
}