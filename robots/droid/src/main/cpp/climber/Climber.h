
#include <Subsystem.h>
#include <motorencodersubsystem/MotorEncoderSubsystem.h>
#include <frc/Servo.h>

namespace xero {
    namespace droid {
        class Climber : public xero::base::Subsystem {
        public:
            Climber(xero::base::Subsystem *parent);
            virtual ~Climber() {}
            
            std::shared_ptr<xero::base::MotorEncoderSubsystem> getLifter() {
                return lifter_ ;
            }
            std::shared_ptr<frc::Servo> getServoR() {
                return servo_r_ ;
            }
            std::shared_ptr<frc::Servo> getServoL() {
                return servo_l_ ;
            }

        private:
            std::shared_ptr<xero::base::MotorEncoderSubsystem> lifter_;
            std::shared_ptr<frc::Servo> servo_r_ ;
            std::shared_ptr<frc::Servo> servo_l_ ;
            //include "subsystem"...?

        private:
            uint64_t msg_id_;

        };
    }
}