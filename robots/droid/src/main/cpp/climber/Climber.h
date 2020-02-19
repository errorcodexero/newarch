#pragma once

#include <Subsystem.h>
#include <motorencodersubsystem/MotorEncoderSubsystem.h>
#include <frc/Servo.h>
#include <frc/PWMSparkMax.h>

namespace xero {
    namespace droid {
        class Climber : public xero::base::Subsystem {
            friend class ClimberTraverseAction ;
            
        public:
            Climber(xero::base::Subsystem *parent);
            virtual ~Climber() {}
            
            std::shared_ptr<xero::base::MotorEncoderSubsystem> getLifter() {
                return lifter_ ;
            }
            
            void setRightServo(double p) {
                servo_r_->Set(p) ;
            }

            void setLeftServo(double p) {
                servo_l_->Set(p) ;
            }

            void setTraverserPower(double p) {
                traverser_->Set(p) ;
            }

        private:
            std::shared_ptr<xero::base::MotorEncoderSubsystem> lifter_;
            std::shared_ptr<frc::Servo> servo_r_ ;
            std::shared_ptr<frc::Servo> servo_l_ ;
            std::shared_ptr<frc::PWMSparkMax> traverser_ ;

        private:
            uint64_t msg_id_;

        };
    }
}

//servo : hitec hs-422
