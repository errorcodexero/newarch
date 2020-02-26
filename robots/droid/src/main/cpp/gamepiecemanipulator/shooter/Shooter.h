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

            bool isReadyToFire();

            void setHood(HoodPosition pos);

            virtual void run() ;

            bool canAcceptAction(xero::base::ActionPtr action);

            bool isHoodReady() ;

        private:
            void updateHood() ;

        private:
            std::shared_ptr<frc::Servo> hoodServo_;

            double hoodUpPos_;
            double hoodDownPos_;

            HoodPosition desired_ ;
            HoodPosition actual_ ;

            double change_time_ ;

            double hood_down_speed_ ;

            double hood_down_delay_ ;

            bool ready_to_fire_ ;      // Set to true when shooter at the required shooting velocity
        };
    }
}