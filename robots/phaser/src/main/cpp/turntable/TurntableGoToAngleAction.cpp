#include "TurntableGoToAngleAction.h"
#include "Turntable.h"
#include <Robot.h>
#include <MessageLogger.h>
#include <cmath>

using namespace xero::misc ;

namespace xero {
    namespace phaser {
        TurntableGoToAngleAction::TurntableGoToAngleAction(Turntable &turntable, double target) : TurntableAction(turntable) {
            angle_threshold_ = getTurntable().getRobot().getSettingsParser().getDouble("turntable:goto:angle_threshold") ;
            speed_threshold_ = getTurntable().getRobot().getSettingsParser().getDouble("turntable:goto:speed_threshold") ;
            target_ = target ;
            pidctrl_.initFromSettingsExtended(getTurntable().getRobot().getSettingsParser(), "turntable:goto") ;          
        }

        TurntableGoToAngleAction::TurntableGoToAngleAction(Turntable &turntable, const std::string &name) : TurntableAction(turntable) {
            angle_threshold_ = getTurntable().getRobot().getSettingsParser().getDouble("turntable:goto:angle_threshold") ;
            speed_threshold_ = getTurntable().getRobot().getSettingsParser().getDouble("turntable:goto:speed_threshold") ;
            target_ = getTurntable().getRobot().getSettingsParser().getDouble(name) ;
            pidctrl_.initFromSettingsExtended(getTurntable().getRobot().getSettingsParser(), "turntable:goto") ;          
        }

        TurntableGoToAngleAction::~TurntableGoToAngleAction() {
        }

        void TurntableGoToAngleAction::start() {
            is_done_ = atTarget() ;
            if (is_done_) {
                Turntable &turntable = getTurntable() ;
                MessageLogger &logger = turntable.getRobot().getMessageLogger() ;              
                logger.startMessage(MessageLogger::MessageType::debug, getTurntable().getMsgID()) ;
                logger << "GoToAngle: start - already at desired target" ;
                logger << ", angle " << getTurntable().getAngleValue() ;
                logger << ", speed " << getTurntable().getVelocity() ;
                logger.endMessage() ;
            }
            start_time_ = getTurntable().getRobot().getTime() ;
        }

        void TurntableGoToAngleAction::run() {
            Turntable &turntable = getTurntable() ;
            MessageLogger &logger = turntable.getRobot().getMessageLogger() ;
            double dt = turntable.getRobot().getDeltaTime() ;
            double elapsed = turntable.getRobot().getTime() - start_time_ ;
            double angle = turntable.getAngleValue() ;
            double speed = turntable.getVelocity() ;

            if (is_done_ || atTarget()) {
                if (!is_done_) {        
                    logger.startMessage(MessageLogger::MessageType::debug, getTurntable().getMsgID()) ;
                    logger << "GoToAngle: complete" ;
                    logger << ", time " << elapsed ;
                    logger << ", angle " << angle ;
                    logger << ", speed " << speed ;
                    logger.endMessage() ;
                }

                turntable.setMotorPower(0.0) ;
                is_done_ = true ;
            }
            else {
                if (!turntable.isCalibrated()) {
                    logger.startMessage(MessageLogger::MessageType::debug, getTurntable().getMsgID()) ;
                    logger << "GoToAngle: turntable not calibrated, aborting go to angle" ;
                    logger.endMessage() ;
                    is_done_ = true ;
                }
                else {
                    output_ = pidctrl_.getOutput(target_, angle, 0, dt) ;
                    turntable.setMotorPower(output_) ;

                    logger.startMessage(MessageLogger::MessageType::debug, getTurntable().getMsgID()) ;
                    logger << "GoToAngle: running" ;
                    logger << ", time " << elapsed ;
                    logger << ", output " << output_ ;
                    logger << ", angle " << angle ;
                    logger << ", target " << target_ ;
                    logger << ", speed " << speed ;
                    logger.endMessage() ;               
                }
            }
        }

        bool TurntableGoToAngleAction::isDone() {
            return is_done_ ;
        }

        void TurntableGoToAngleAction::cancel() {
            is_done_ = true ;
        }

        bool TurntableGoToAngleAction::atTarget() {
            return 
                std::fabs(getTurntable().getAngleValue() - target_) < angle_threshold_ &&
                std::fabs(getTurntable().getVelocity()) < speed_threshold_ ;
        }

        std::string TurntableGoToAngleAction::toString() {
            std::string result = "TurntableGoToAngle " + std::to_string(target_) ;
            return result ;
        }
    }
}