#include "TurntableHoldAngleAction.h"
#include "Turntable.h"
#include <Robot.h>
#include <MessageLogger.h>
#include <cmath>

using namespace xero::misc ;

namespace xero {
    namespace phaser {
        TurntableHoldAngleAction::TurntableHoldAngleAction(Turntable &turntable, double target) : TurntableAction(turntable) {
            target_ = target ;
            pidctrl_.initFromSettingsExtended(getTurntable().getRobot().getSettingsParser(), "turntable:hold") ;          
        }

        TurntableHoldAngleAction::TurntableHoldAngleAction(Turntable &turntable, const std::string &name) : TurntableAction(turntable) {
            target_ = getTurntable().getRobot().getSettingsParser().getDouble(name) ;
            pidctrl_.initFromSettingsExtended(getTurntable().getRobot().getSettingsParser(), "turntable:hold") ;          
        }

        TurntableHoldAngleAction::~TurntableHoldAngleAction() {
        }

        void TurntableHoldAngleAction::start() {
            start_time_ = getTurntable().getRobot().getTime() ;
            is_done_ = false ;
        }

        void TurntableHoldAngleAction::run() {
            Turntable &turntable = getTurntable() ;
            MessageLogger &logger = turntable.getRobot().getMessageLogger() ;
            double dt = turntable.getRobot().getDeltaTime() ;
            double elapsed = turntable.getRobot().getTime() - start_time_ ;
            double angle = turntable.getAngleValue() ;
            double speed = turntable.getVelocity() ;

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

        bool TurntableHoldAngleAction::isDone() {
            return is_done_ ;
        }

        void TurntableHoldAngleAction::cancel() {
            is_done_ = true ;
        }

        std::string TurntableHoldAngleAction::toString() {
            std::string result = "TurntableGoToAngle " + std::to_string(target_) ;
            return result ;
        }
    }
}