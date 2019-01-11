#pragma once

#include <Subsystem.h>
#include <frc/VictorSP.h>
#include <frc/Encoder.h>
#include <frc/DigitalInput.h>
#include <frc/Solenoid.h>
#include "XeroTalonSRX.h"

namespace xero {
    namespace base {
        class Lifter : public xero::base::Subsystem {
            friend class LifterGoToHeightAction ;
            friend class LifterPowerAction ;
            friend class LifterCalibrateAction ;

        public:
            Lifter(xero::base::Robot &robot, uint64_t id) ;
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

            bool isAtTop() {
                return top_limit_switch_ ;
            }

            bool isAtBottom() {
                return bottom_limit_switch_ ;
            }

            bool isCalibrated() {
                return is_calibrated_ ;
            }

        private:            
            void calibrate() ;
            void setMotorPower(double v) ;

            uint64_t getMsgID() const {
                return msg_id_ ;
            }

            void getMotors(Robot &robot) ;

        private:
            std::list<std::shared_ptr<TalonSRX>> motors_ ;
            std::shared_ptr<frc::Encoder> encoder_ ;
            std::shared_ptr<frc::DigitalInput> bottom_limit_ ;
            std::shared_ptr<frc::DigitalInput> top_limit_ ;

            bool is_calibrated_ ;
            bool top_limit_switch_ ;
            bool bottom_limit_switch_ ;
            bool calibrate_from_limit_switch_ ;

            int encoder_value_ ;

            double lifter_offset ;
            double inches_per_tick_ ;
            double height_ ;
            double last_height_ ;
            double speed_ ;
            double max_height_ ;
            double min_height_ ;

            uint64_t msg_id_ ;
        } ;
    }
}