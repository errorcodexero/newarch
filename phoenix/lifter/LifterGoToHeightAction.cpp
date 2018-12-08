#include "LifterGoToHeightAction.h"
#include "Lifter.h"
#include "phoenixgroups.h"
#include <Robot.h>
#include <MessageLogger.h>
#include <cmath>

using namespace xero::misc ;

namespace xero {
    namespace phoenix {
        LifterGoToHeightAction::LifterGoToHeightAction(Lifter &lifter, double target) : LifterAction(lifter) {
            height_threshold_ = getLifter().getRobot().getSettingsParser().getDouble("lifter:goto:distance_threshold") ;
            speed_threshold_ = getLifter().getRobot().getSettingsParser().getDouble("lifter:goto:speed_threshold") ;
            target_ = target ;
			pidctrl_.initFromSettingsExtended(getLifter().getRobot().getSettingsParser(), "lifter:goto") ;			
        }

        LifterGoToHeightAction::LifterGoToHeightAction(Lifter &lifter, const std::string &name) : LifterAction(lifter) {
            height_threshold_ = getLifter().getRobot().getSettingsParser().getDouble("lifter:goto:distance_threshold") ;
            speed_threshold_ = getLifter().getRobot().getSettingsParser().getDouble("lifter:goto:speed_threshold") ;
            target_ = getLifter().getRobot().getSettingsParser().getDouble(name) ;
			pidctrl_.initFromSettingsExtended(getLifter().getRobot().getSettingsParser(), "lifter:goto") ;			
        }

        LifterGoToHeightAction::~LifterGoToHeightAction() {
        }

        void LifterGoToHeightAction::start() {
            is_done_ = atTarget() ;
			if (is_done_) {
            	Lifter &lifter = getLifter() ;
				MessageLogger &logger = lifter.getRobot().getMessageLogger() ;				
				logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_LIFTER) ;
				logger << "GoToHeight: start - already at desired target" ;
				logger << ", height " << getLifter().getHeight() ;
				logger << ", speed " << getLifter().getVelocity() ;
				logger.endMessage() ;
			}
			start_time_ = getLifter().getRobot().getTime() ;
        }

        void LifterGoToHeightAction::run() {
            Lifter &lifter = getLifter() ;
			MessageLogger &logger = lifter.getRobot().getMessageLogger() ;
            double dt = lifter.getRobot().getDeltaTime() ;
			double elapsed = lifter.getRobot().getTime() - start_time_ ;
			double height = lifter.getHeight() ;
			double speed = lifter.getVelocity() ;

            if (is_done_ || atTarget()) {
				if (!is_done_) {		
					logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_LIFTER) ;
					logger << "GoToHeight: complete" ;
					logger << ", time " << elapsed ;
					logger << ", height " << height ;
					logger << ", speed " << speed ;
					logger.endMessage() ;
				}

                lifter.setBrakeOn() ;
				lifter.setMotorDutyCycle(0.0) ;
				is_done_ = true ;
            }
            else {
				if (!lifter.isCalibrated()) {
					logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_LIFTER) ;
					logger << "GoToHeight: lifter not calibrated, aborting go to height" ;
					logger.endMessage() ;
					is_done_ = true ;
				}
				else {
					output_ = pidctrl_.getOutput(target_, height, 0, dt) ;
					lifter.setBrakeOff() ;
					lifter.setMotorDutyCycle(output_) ;

					logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_LIFTER) ;
					logger << "GoToHeight: running" ;
					logger << ", time " << elapsed ;
					logger << ", output " << output_ ;
					logger << ", height " << height ;
					logger << ", target " << target_ ;
					logger << ", speed " << speed ;
					logger.endMessage() ;				
				}
            }
        }

        bool LifterGoToHeightAction::isDone() {
            return is_done_ ;
        }

        void LifterGoToHeightAction::cancel() {
            is_done_ = true ;
        }

        bool LifterGoToHeightAction::atTarget() {
            return 
                std::fabs(getLifter().getHeight() - target_) < height_threshold_ &&
                std::fabs(getLifter().getVelocity()) < speed_threshold_ ;
        }

        std::string LifterGoToHeightAction::toString() {
            std::string result = "LifterGoToHeight " + std::to_string(target_) ;
            return result ;
        }
    }
}