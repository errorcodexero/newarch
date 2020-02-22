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

            void setTraverserPower(double p) {
                traverser_->Set(p) ;
            }

            double getMaxHeight() {
                return max_height_ ;
            }

        private:
            std::shared_ptr<xero::base::MotorEncoderSubsystem> lifter_;
            std::shared_ptr<frc::PWMSparkMax> traverser_ ;
            uint64_t msg_id_;
            double max_height_ ;

        };
    }
}

//servo : hitec hs-422
