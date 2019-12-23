#include "Lifter.h"
#include "LifterAction.h"
#include "phoenixids.h"
#include <Robot.h>
#include <MessageLogger.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace phoenix {
        Lifter::Lifter(Subsystem *parent) : Subsystem(parent, "lifter") {
            auto &robot = getRobot();

            int m1 = robot.getSettingsParser().getInteger("hw:lifter:motor1");
            int m2 = robot.getSettingsParser().getInteger("hw:lifter:motor2");
            int e1 = robot.getSettingsParser().getInteger("hw:lifter:encoder1");
            int e2 = robot.getSettingsParser().getInteger("hw:lifter:encoder2");
            int lbottom = robot.getSettingsParser().getInteger("hw:lifter:limit:bottom");
            int ltop = robot.getSettingsParser().getInteger("hw:lifter:limit:top");
            int s1 = robot.getSettingsParser().getInteger("hw:lifter:shifter");
            int s2 = robot.getSettingsParser().getInteger("hw:lifter:brake") ;

            motor1_ = std::make_shared<frc::VictorSP>(m1) ;
            motor2_ = std::make_shared<frc::VictorSP>(m2) ;
            encoder_ = std::make_shared<frc::Encoder>(e1, e2) ;
            bottom_limit_ = std::make_shared<frc::DigitalInput>(lbottom) ;
            top_limit_ = std::make_shared<frc::DigitalInput>(ltop) ;
            gear_box_ = std::make_shared<frc::Solenoid>(s1) ;
            brake_ = std::make_shared<frc::Solenoid>(s2) ;

            collector_offset_ = robot.getSettingsParser().getDouble("lifter:base") ;
            inches_per_tick_high_ = robot.getSettingsParser().getDouble("lifter:inches_per_tick") ;
            max_height_ = robot.getSettingsParser().getDouble("lifter:max_height") ;
            min_height_ = robot.getSettingsParser().getDouble("lifter:min_height") ;

            // Start in high gear
            setHighGear() ;

            // Start with the brake on
            setBrakeOn() ;

            // And we start without calibration
            is_calibrated_ = false ;

            encoder_value_ = 0 ;
        }

        Lifter::~Lifter() {
        }

        bool Lifter::canAcceptAction(ActionPtr action) {
            auto dir_p = std::dynamic_pointer_cast<LifterAction>(action) ;
            if (dir_p == nullptr)
                return false ;

            return true ;
        }

        void Lifter::setMotorDutyCycle(double v) {
            if (is_calibrated_) {
                if (v < 0 && height_ <= min_height_)
                    v = 0.0 ;
                else if (v > 0 && height_ >= max_height_)
                    v = 0.0 ;
            }

            if (v > 0 && isAtTop())
                v = 0.0 ;
            else if (v < 0 && isAtBottom())
                v = 0.0 ;

            motor1_->Set(v) ;
            motor2_->Set(v) ;
        }

        bool Lifter::isAtBottom() {
            if (is_calibrated_ && height_ <= min_height_) {
                return true ;
            }

            return !bottom_limit_->Get() ;
        }

        bool Lifter::isAtTop() {
            if (is_calibrated_ && height_ >= max_height_)
                return true ;

            return !top_limit_->Get() ;
        }

        void Lifter::computeState() {
            encoder_value_ = encoder_->Get() ;

            if (!bottom_limit_->Get() && high_gear_) {
                //
                // The bottom limit switch is is true, reset the encoders and
                // effectively recalibrate
                //
                calibrate() ;
            }           

            if (is_calibrated_) {
                height_ = encoder_value_ * inches_per_tick_high_ + collector_offset_ ;
                speed_ = (height_ - last_height_) / getRobot().getDeltaTime() ;
                last_height_ = height_ ;
            }
        }

        void Lifter::calibrate() {
            //
            // The calibrate action assumes the lifter starts the match
            // at the bottom most position.  This position is the floor position
            // and the calibration activity is just remembering the encoder value
            // at this lifter position
            //
            if (is_calibrated_ == false) {
                is_calibrated_ = true ;
                MessageLogger &logger = getRobot().getMessageLogger() ;
                logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_LIFTER) ;
                logger << "Lifter: lifter calibrated" ;
                logger.endMessage() ;
            }
            encoder_->Reset() ;
            last_height_ = collector_offset_ ;
        }

        void Lifter::setLowGear() {
            if (high_gear_) {
                gear_box_->Set(true) ;
                high_gear_ = false ;
                is_calibrated_ = false ;
            }
        }

        void Lifter::setHighGear() {
            if (!high_gear_) {
                gear_box_->Set(false) ;
                high_gear_ = true ;
                is_calibrated_ = false ;
            }
        }

        void Lifter::setBrakeOn() {
            if (!brake_on_) {
                brake_->Set(false) ;
                brake_on_ = true ;
            }
        }

        void Lifter::setBrakeOff() {
            if (brake_on_) {
                brake_->Set(true) ;
                brake_on_ = false ;
            }
        }
    }
}
