#include "Turntable.h"
#include "TurntableAction.h"
#include "TurntableCalibrateAction.h"
#include <frc/smartdashboard/SmartDashboard.h>
#include <Robot.h>
#include <MessageLogger.h>
#include <xeromath.h>
#include <cmath>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero{
    namespace phaser{
        Turntable::Turntable(Robot &robot, Lifter &lifter, uint64_t id) : Subsystem(robot, "turntable"), lifter_(lifter) {

            SettingsParser &parser = robot.getSettingsParser() ;
            msg_id_ = id ;
            
            getMotors(robot) ;
            assert(motors_.size() > 0) ;

            int enc1 = robot.getSettingsParser().getInteger("hw:turntable:encoder1") ;
            int enc2 = robot.getSettingsParser().getInteger("hw:turntable:encoder2") ;           
            encoder_ = std::make_shared<frc::Encoder>(enc1, enc2) ;
            encoder_->SetReverseDirection(true) ;

            min_angle_ = robot.getSettingsParser().getDouble("turntable:keepout:minimum") ;
            max_angle_ = robot.getSettingsParser().getDouble("turntable:keepout:maximum") ;
            danger_zone_ = robot.getSettingsParser().getDouble("turntable:keepout:dangerzone") ;
            degrees_per_tick_ = robot.getSettingsParser().getDouble("turntable:degrees_per_tick") ;

            //
            // The angle of the turntable when the encoders are at zero, the calibration
            // point.
            //
            turntable_offset_ = parser.getDouble("turntable:base") ;

            safe_rotate_height_ = parser.getDouble("turntable:safe_lifter_height") ;
            
            // And we start without calibration
            is_calibrated_ = true ;

            angle_ = 0.0 ;
            last_angle_ = 0.0 ;

            loops_ = 0 ;
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
                auto talon = std::make_shared<TalonSRX>(motor) ;
                talon->SetInverted(reverse) ;
                talon->ConfigVoltageCompSaturation(12.0, 10) ;
                talon->EnableVoltageCompensation(true) ;                
                talon->SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake) ;

                if (motors_.size() > 0)
                    talon->Follow(*motors_.front()) ;

                motors_.push_back(talon) ;

                i++ ;
            }
        }

        void Turntable::setMotorPower(double v) {
            if (is_calibrated_) {
                if (angle_ >= min_angle_ - danger_zone_ && angle_ <= min_angle_ && v > 0)
                    v = 0 ;
                
                if (angle_ <= max_angle_ + danger_zone_ && angle_ >= max_angle_ && v < 0)
                    v = 0 ;
            }
            
            if (motors_.size() > 0)
                motors_.front()->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, v);
        }

        bool Turntable::canAcceptAction(ActionPtr action) {
            return true ;
            
            auto dir_p = std::dynamic_pointer_cast<TurntableAction>(action) ;
            if (dir_p == nullptr)
                return false ;

            auto calib_p = std::dynamic_pointer_cast<TurntableCalibrateAction>(action) ;
            if (calib_p != nullptr)
                return true ;

            if (!lifter_.isCalibrated() || lifter_.getHeight() < getSafeRotateHeight()) {
                MessageLogger &logger = getRobot().getMessageLogger() ;
                logger.startMessage(MessageLogger::MessageType::error) ;
                logger << "Turntable: canAcceptAction failed, lifter to to low to rotate turntable\n" ;
                logger << "           " << action->toString() ;
                logger.endMessage() ;
                return false ;
            }

            return true ;
        }

        void Turntable::computeState(){
            encoder_value_ = encoder_->Get() ;
            if (is_calibrated_) {
                angle_ = xero::math::normalizeAngleDegrees(encoder_value_ * degrees_per_tick_ + turntable_offset_) ;
                speed_ = (angle_ - last_angle_) / getRobot().getDeltaTime() ;
                last_angle_ = angle_ ;
            }

            auto &logger = getRobot().getMessageLogger() ;
            logger.startMessage(MessageLogger::MessageType::debug, msg_id_) ;
            logger << "Turntable:" ;
            logger << " ticks " << encoder_value_ ;
            logger << " calibrated " << is_calibrated_ ;
            logger << " angle " << angle_ ;
            logger << " speed " << speed_ ;  
            logger.endMessage() ; 

            frc::SmartDashboard::PutNumber("Turntable", angle_) ;                       
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