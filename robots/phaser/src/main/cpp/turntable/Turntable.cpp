#include "Turntable.h"
#include "TurntableAction.h"
#include <Robot.h>
#include <MessageLogger.h>
#include <xeromath.h>
#include <cmath>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero{
    namespace phaser{
        Turntable::Turntable(Robot &robot, uint64_t id) : Subsystem(robot, "turntable") {

            SettingsParser &parser = robot.getSettingsParser() ;
            
            msg_id_ = id ;
            
            getMotors(robot) ;
            assert(motors_.size() > 0) ;

            int enc1 = robot.getSettingsParser().getInteger("hw:turntable:encoder1") ;
            int enc2 = robot.getSettingsParser().getInteger("hw:turntable:encoder2") ;           
            encoder_ = std::make_shared<frc::Encoder>(enc1, enc2) ;

            min_angle_ = robot.getSettingsParser().getDouble("turntable:keepout:minimum") ;
            max_angle_ = robot.getSettingsParser().getDouble("turntable:keepout:maximum") ;
            degrees_per_tick_ = robot.getSettingsParser().getDouble("turntable:degrees_per_tick") ;

            //
            // The angle of the turntable when the encoders are at zero, the calibration
            // point.
            //
            turntable_offset_ = parser.getDouble("turntable:base") ;
            
            // And we start without calibration
            is_calibrated_ = false ;
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

                i++ ;
            }
        }

        void Turntable::setMotorPower(double v) {
            if (is_calibrated_) {
                if (angle_ >= min_angle_ && angle_ <= max_angle_) {
                    //
                    // We are in the danger zone.  Should only move the turntable
                    // to get out of the danger zone
                    //
                    double d1 = std::fabs(angle_ - min_angle_) ;
                    double d2 = std::fabs(angle_ - max_angle_) ;

                    if (d1 < d2) {
                        // Closest to the min angle point
                        if (v < 0)
                            v = 0 ;
                    }
                    else {
                        // closeest to the max angle point
                        if (v > 0)
                            v = 0 ;
                    }
                }
            }

            if (motors_.size() > 0)
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
            last_angle_ = 0.0 ;
        }
    }
}