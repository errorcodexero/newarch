#pragma once

#include <Subsystem.h>
#include <frc/VictorSP.h>
#include <frc/Encoder.h>
#include <frc/DigitalInput.h>

namespace xero {
    namespace phaser{
        class Turntable: public xero::base::Subsystem {
        
        public:
             Turntable(xero::base::Robot &robot) ;
             virtual ~Turntable() ;

             virtual bool canAcceptAction(xero::phaser::ActionPtr action) ;
             virtual bool computeState() ;    

             int getEncoderValue() const {
                return encoder_value_ ;
            }

            double getAngleValue() const {
                return angle_;
            }

        private:
            std::shared_ptr<frc::VictorSP> motor_ ;
            std::shared_ptr<frc::Encoder> encoder_ ;
            std::shared_ptr<frc::DigitalInput> min_angle_switch_ ;
            std::shared_ptr<frc::DigitalInput> max_angle_switch_ ;

            int encoder_value_;
            double min_angle_;
            double max_angle_;
            double degrees_per_tick_;
            double angle_;
        };
    }
}