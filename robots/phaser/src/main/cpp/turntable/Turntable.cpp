#include "Turntable.h"
#include "TurntableAction.h"
#include "TurntableCalibrateAction.h"
#include "TurntableGoToAngleAction.h"
#include <frc/smartdashboard/SmartDashboard.h>
#include <Robot.h>
#include <motors/MotorFactory.h>
#include <motors/MotorController.h>
#include <MessageLogger.h>
#include <xeromath.h>
#include <cmath>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero{
    namespace phaser{
        Turntable::Turntable(Robot &robot, Lifter &lifter, uint64_t id, uint64_t verboseid) : Subsystem(robot, "turntable"), lifter_(lifter) {

            SettingsParser &parser = robot.getSettingsParser() ;
            msg_id_ = id ;
            msg_verbose_id_ = verboseid ;
            
            getMotors(robot) ;

            int enc1 = robot.getSettingsParser().getInteger("hw:turntable:encoder1") ;
            int enc2 = robot.getSettingsParser().getInteger("hw:turntable:encoder2") ;           
            encoder_ = std::make_shared<frc::Encoder>(enc1, enc2) ;
            encoder_->SetReverseDirection(true) ;
            encoder_->Reset() ;

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
            safe_rotate_margin_= parser.getDouble("turntable:safe_lifter_margin") ;
            
            calibrate(0) ;
            loops_ = 0 ;

            //
            // Hold the turntable at its starting position
            //
            auto action = std::make_shared<TurntableGoToAngleAction>(*this, angle_) ;
            setAction(action) ;
        }

        Turntable::~Turntable() {
        }

        void Turntable::getMotors(Robot &robot) {
            bool reverse = false ;

            std::string revname = "hw:turntable:reversemotors" ;

            if (robot.getSettingsParser().isDefined(revname)) {
                if (robot.getSettingsParser().get(revname).isBoolean()) {
                    if (robot.getSettingsParser().getBoolean(revname)) {
                        reverse = true ;
                    }
                }
            }

            motors_ = robot.getMotorFactory()->createMotor("hw:turntable:motors");
            motors_->setInverted(reverse);
            motors_->setNeutralMode(MotorController::NeutralMode::Brake);
        }

        void Turntable::setMotorPower(double v) {
            if (is_calibrated_) {
                if (angle_ >= min_angle_ - danger_zone_ && angle_ <= min_angle_ && v > 0)
                    v = 0 ;
                
                if (angle_ <= max_angle_ + danger_zone_ && angle_ >= max_angle_ && v < 0)
                    v = 0 ;
            }
            
            motors_->set(v);
        }

        bool Turntable::canAcceptAction(ActionPtr action) {
            return true ;
            
            auto dir_p = std::dynamic_pointer_cast<TurntableAction>(action) ;
            if (dir_p == nullptr)
                return false ;

            auto calib_p = std::dynamic_pointer_cast<TurntableCalibrateAction>(action) ;
            if (calib_p != nullptr)
                return true ;

            if (!lifter_.isCalibrated()) {
                MessageLogger &logger = getRobot().getMessageLogger() ;
                logger.startMessage(MessageLogger::MessageType::error) ;
                logger << "Turntable: canAcceptAction failed, lifter not calibrated\n" ;
                logger << "           " << action->toString() ;
                logger.endMessage() ;
                return false ;
            }

            //
            // We assume no hatch here.  This is a safety check to ensure we don't destroy
            // the robot.  Higher level systems should ensure we are at the right height to
            // rotate without losing a hatch.
            //
            if (!isSafeToRotate()) {
                MessageLogger &logger = getRobot().getMessageLogger() ;
                logger.startMessage(MessageLogger::MessageType::error) ;
                logger << "Turntable: canAcceptAction failed, lifter too low to rotate\n" ;
                logger << "           " << action->toString() ;
                logger.endMessage() ;
                return false ;
            }

            return true ;
        }

        void Turntable::computeState(){
            encoder_value_ = encoder_->Get() ;
            if (is_calibrated_) {
                angle_ = xero::math::normalizeAngleDegrees((encoder_value_ + encoder_base_) * degrees_per_tick_ + turntable_offset_) ;
                speed_ = (angle_ - last_angle_) / getRobot().getDeltaTime() ;
                last_angle_ = angle_ ;
            }

            auto &logger = getRobot().getMessageLogger() ;
            logger.startMessage(MessageLogger::MessageType::debug, msg_verbose_id_) ;
            logger << "Turntable:" ;
            logger << " ticks " << encoder_value_ ;
            logger << " encbase " << encoder_base_ ;
            logger << " calibrated " << is_calibrated_ ;
            logger << " angle " << angle_ ;
            logger << " speed " << speed_ ;  
            logger.endMessage() ; 

            frc::SmartDashboard::PutNumber("Turntable", angle_) ;                       
        }

        void Turntable::calibrate(int cal) {
            //
            // The calibrate action assumes the turntable is at the bottom of travel
            // This may be because the turntable put there at the start of the match
            // and calibrate is called.  It may also be because the bottom limit
            // has activated and we are recalibrating to the limit switch
            //
            is_calibrated_ = true ;
            angle_ = 0.0 ;
            last_angle_ = 0.0 ;
            encoder_base_ = cal ;

            encoder_->Reset() ;

            auto &logger = getRobot().getMessageLogger() ;
            logger.startMessage(MessageLogger::MessageType::info) ;
            logger << "Turntable: calibrated with encoder base " << encoder_base_ ;
            logger.endMessage() ;
        }
    }
}