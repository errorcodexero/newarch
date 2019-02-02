#include "Turntable.h"
#include "TurntableAction.h"
#include <Robot.h>
#include <MessageLogger.h>
#include <xeromath.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero{
    namespace phaser{
        Turntable::Turntable(Robot &robot, uint64_t id) : Subsystem(robot, "turntable") {

            SettingsParser &parser = robot.getSettingsParser() ;
            
            msg_id_ = id ;
            
            getMotors(robot) ;

            int enc1 = robot.getSettingsParser().getInteger("hw:turntable:encoder1") ;
            int enc2 = robot.getSettingsParser().getInteger("hw:turntable:encoder2") ;           
            encoder_ = std::make_shared<frc::Encoder>(enc1, enc2) ;

            min_angle_ = robot.getSettingsParser().getDouble("turntable:minimum") ;
            max_angle_ = robot.getSettingsParser().getDouble("turntable:maximum") ;
            degrees_per_tick_ = robot.getSettingsParser().getDouble("turntable:degrees_per_tick") ;
            if (parser.isDefined("hw:turntable:limit:min_angle")) {
                int lbottom = parser.getInteger("hw:turntable:limit:min_angle");
                min_angle_switch_ = std::make_shared<frc::DigitalInput>(lbottom) ;
            }

            if (parser.isDefined("hw:turntable:limit:max_angle")) {
                int ltop = parser.getInteger("hw:turntable:limit:max_angle");
                max_angle_switch_ = std::make_shared<frc::DigitalInput>(ltop) ;
            }

            //
            // The height of the turntable when it is at the bottom of travel, relative to 
            // the floor
            //
            turntable_offset_ = parser.getDouble("turntable:base") ;
            
            // And we start without calibration
            is_calibrated_ = false ;

            // The base encoder value
            encoder_value_ = 0 ;

            // Initialize to false, only gets reset if we have a limit switch
            is_max_angle_ = false ;
            is_min_angle_ = false ;
        }

        Turntable::~Turntable() {
        }

        void Turntable::getMotors(Robot &robot) {
            int i = 1 ;
            bool reverse = false ;

            std::string revname = "hw:turntable:reversemotors" ;

            if (robot.getSettingsParser().isDefined(revname)) {
                if (robot.getSettingsParser().get(revname).isBoolean()) {
                    if (robot.getSettingsParser().getBoolean(revname)) {
                        reverse = true ;
                    }
                }
            }

            while (true) {
                std::string motorname = "hw:turntable:motor:" + std::to_string(i) ;
                if (!robot.getSettingsParser().isDefined(motorname))
                    break ;

                int motor = robot.getSettingsParser().getInteger(motorname) ;
                auto talon = std::make_shared<ctre::phoenix::motorcontrol::can::TalonSRX>(motor) ;
                talon->SetInverted(reverse) ;
                talon->ConfigVoltageCompSaturation(12.0, 10) ;
                talon->EnableVoltageCompensation(true) ;                

                if (motors_.size() > 0)
                    talon->Follow(*motors_.front()) ;

                motors_.push_back(talon) ;
            }
        }

        void Turntable::setMotorPower(double v) {
            if (is_calibrated_) {
                //
                // If we are calibrated, we know our height.  If our height exceeds
                // the maximum allow height and we are trying to move up, we set the
                // power to zero
                //
                double angdiff = xero::math::normalizeAngleDegrees(angle_ - min_angle_) ;
                if (v < 0.0 && angdiff < 0.0)
                    v = 0.0 ;

                angdiff = xero::math::normalizeAngleDegrees(max_angle_ - angle_) ;
                if (v > 0 && angdiff < 0.0)
                    v = 0.0 ;
            }

            //
            // Now check the raw limit switch states
            //
            if (v > 0 && isMaxAngle())
                v = 0.0 ;
            else if (v < 0 && isMinAngle())
                v = 0.0 ;

            motors_.front()->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, v);
        }

        bool Turntable::canAcceptAction(ActionPtr action) {
            auto dir_p = std::dynamic_pointer_cast<TurntableAction>(action) ;
            if (dir_p == nullptr)
                return false ;

            return true ;
        }

        void Turntable::computeState(){
            encoder_value_ = encoder_->Get() ;
            if (min_angle_switch_ != nullptr)
                is_min_angle_ = min_angle_switch_->Get() ;

            if (max_angle_switch_ != nullptr)
                is_max_angle_ = max_angle_switch_->Get() ;

            if (is_calibrated_) {
                angle_ = xero::math::normalizeAngleDegrees(encoder_value_ * degrees_per_tick_ + turntable_offset_) ;
                speed_ = (angle_ - last_angle_) / getRobot().getDeltaTime() ;
                last_angle_ = angle_ ;
            }
        }

        void Turntable::calibrate() {
            //
            // The calibrate action assumes the turntable is at the bottom of travel
            // This may be because the turntable put there at the start of the match
            // and calibrate is called.  It may also be because the bottom limit
            // has activated and we are recalibrating to the limit switch
            //
            is_calibrated_ = true ;
            encoder_->Reset() ;
            last_angle_ = turntable_offset_ ;
        }
    }
}