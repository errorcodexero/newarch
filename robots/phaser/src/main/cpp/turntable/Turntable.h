#pragma once

#include <Subsystem.h>
#include <lifter/Lifter.h>
#include <ctre/Phoenix.h>
#include <frc/Encoder.h>
#include <frc/DigitalInput.h>
typedef ctre::phoenix::motorcontrol::can::TalonSRX TalonSRX;

namespace xero {
    namespace phaser{
        class Turntable: public xero::base::Subsystem {

            friend class TurntableCalibrateAction ;
            friend class TurntableGoToAngleAction ;
            friend class TurntablePowerAction ;
        
        public:
             Turntable(xero::base::Robot &robot, xero::base::Lifter &lifter, uint64_t id, uint64_t verboseid) ;
             virtual ~Turntable() ;

             virtual bool canAcceptAction(xero::base::ActionPtr action) ;
             virtual void computeState() ;    

             int getEncoderValue() const {
                return encoder_value_ ;
            }

            double getAngleValue() const {
                return angle_;
            }

            double getVelocity() const {
                return speed_ ;
            }

            double getMinKeepOutAngle() const {
                return min_angle_ ;
            }

            double gettMaxKeepoutAngle() const {
                return max_angle_ ;
            }

            bool isCalibrated() {
                return is_calibrated_ ;
            }

        private:
            void calibrate() ;
            void setMotorPower(double v) ;

            double getSafeRotateHeight() {
                return safe_rotate_height_ ;
            }

            xero::base::Lifter &getLifter() {
                return lifter_ ;
            }

            uint64_t getMsgID() const {
                return msg_id_ ;
            }

            void getMotors(xero::base::Robot &robot) ;

        private:
            std::list<std::shared_ptr<TalonSRX>> motors_ ;
            std::shared_ptr<frc::Encoder> encoder_ ;

            int encoder_value_;
            double min_angle_;
            double max_angle_;
            double danger_zone_ ;
            double degrees_per_tick_;
            double angle_;
            bool is_calibrated_;
            double speed_;
            int loops_ ;

            //
            // The height required of the lifter before the turntable should be rotated
            //
            double safe_rotate_height_ ;

            //
            // This is the angle of the turntable when the encoder is at
            // zero.  This angle gets added to the calculated angle when
            // encoder values are converted to angle.
            //
            double turntable_offset_;

            //
            // This is the previous angle of the turntable, used to calculate the
            // angular velocity of the turntable
            //
            double last_angle_;

            uint64_t msg_id_ ;
            uint64_t msg_verbose_id_ ;

            xero::base::Lifter &lifter_ ;
        };
    }
}