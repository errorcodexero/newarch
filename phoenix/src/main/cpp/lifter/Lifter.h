#pragma once

#include <Subsystem.h>
#include <frc/VictorSP.h>
#include <frc/Encoder.h>
#include <frc/DigitalInput.h>
#include <frc/Solenoid.h>

namespace xero {
    namespace phoenix {
        class Lifter : public xero::base::Subsystem {
            friend class LifterGoToHeightAction ;
            friend class LifterPowerAction ;
            friend class LifterCalibrateAction ;
            friend class LifterLowGearAction ;
            friend class LifterGoToEncoderAction ;
            friend class LifterHoldClimbAction ;
        public:
            Lifter(xero::base::Robot &robot) ;
            virtual ~Lifter() ;

            virtual bool canAcceptAction(xero::base::ActionPtr action) ;
            virtual void computeState() ;

            double getHeight() const {
                return height_ ;
            }

            double getVelocity() const {
                return speed_ ;
            }

            int getEncoderValue() const {
                return encoder_value_ ;
            }

            bool isAtTop() ;
            bool isAtBottom() ;
            bool isCalibrated() {
                return is_calibrated_ ;
            }

            void createNamedSequences() {
            }

        private:            
            void calibrate() ;
            void setLowGear() ;
            void setHighGear() ;
            void setBrakeOn() ;
            void setBrakeOff() ;
            void setMotorDutyCycle(double v) ;

        private:
            std::shared_ptr<frc::VictorSP> motor1_ ;
            std::shared_ptr<frc::VictorSP> motor2_ ;
            std::shared_ptr<frc::Encoder> encoder_ ;
            std::shared_ptr<frc::DigitalInput> bottom_limit_ ;
            std::shared_ptr<frc::DigitalInput> top_limit_ ;
            std::shared_ptr<frc::Solenoid> gear_box_ ;
            std::shared_ptr<frc::Solenoid> brake_ ;

            bool is_calibrated_ ;

            int encoder_value_ ;

            double collector_offset_ ;
            double inches_per_tick_high_ ;
            double height_ ;
            double last_height_ ;
            double speed_ ;
            double max_height_ ;
            double min_height_ ;

            bool high_gear_ ;
            bool brake_on_ ;
        } ;
    }
}