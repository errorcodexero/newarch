#include "MotorEncoderGoToAction.h"

#include <string>
#include <memory>
#include <cmath>

#include <Robot.h>
#include <MessageLogger.h>

#include "MotorEncoderSubsystem.h"
#include "MotorEncoderHoldAction.h"

using namespace xero::misc ;

namespace xero {
    namespace base {
        std::vector<std::string> MotorEncoderGoToAction::plot_columns_ = {
            "time", 
            "tpos", "apos", "tvel", "avel", "out"
        } ;

        MotorEncoderGoToAction::MotorEncoderGoToAction(MotorEncoderSubsystem &subsystem, double target):
            MotorEncoderSubsystemAction(subsystem) {
            
            std::string config = subsystem.getName() + ":goto";
            auto &settings = subsystem.getRobot().getSettingsParser();
            
            target_ = target;
            threshold_ = settings.getDouble(config + ":threshold");
            
            profile_ = std::make_shared<TrapezoidalProfile>(
                settings.getDouble(config + ":maxa"),
                settings.getDouble(config + ":maxd"),
                settings.getDouble(config + ":maxv")
            );

            plotid_ = subsystem.initPlot(subsystem.getName() + "-" + toString()) ;
        }

        MotorEncoderGoToAction::MotorEncoderGoToAction(MotorEncoderSubsystem &subsystem, const std::string &targetparam):
            MotorEncoderSubsystemAction(subsystem) {
            
            std::string config = subsystem.getName() + ":goto";
            auto &settings = subsystem.getRobot().getSettingsParser();
            
            target_ = subsystem.getRobot().getSettingsParser().getDouble(targetparam) ;
            threshold_ = settings.getDouble(config + ":threshold");
            
            profile_ = std::make_shared<TrapezoidalProfile>(
                settings.getDouble(config + ":maxa"),
                settings.getDouble(config + ":maxd"),
                settings.getDouble(config + ":maxv")
            );

            plotid_ = subsystem.initPlot(subsystem.getName() + "-" + toString()) ;
        }        

        void MotorEncoderGoToAction::start() {
            MotorEncoderSubsystem &subsystem = getSubsystem();
            isDone_ = false ;
            subsystem.setDefaultAction(std::make_shared<MotorEncoderHoldAction>(subsystem, target_));

            double dist = normalizePosition(target_ - subsystem.getPosition());
            if (std::fabs(dist) < threshold_) {
                isDone_ = true;
                MessageLogger &logger = subsystem.getRobot().getMessageLogger();
                logger.startMessage(MessageLogger::MessageType::debug, subsystem.msg_id_);
                logger << "MotorEncoderGoToAction (" ;
                logger << subsystem.getName() ;
                logger << "): action completed sucessfully in start";
                logger.endMessage();
                return;
            }

            std::string config = subsystem.getName() + ":follower";
            auto &settings = subsystem.getRobot().getSettingsParser();

            if (dist < 0) {
                ctrl_ = std::make_shared<PIDACtrl>(
                    settings, 
                    config + ":down:kv",
                    config + ":down:ka",
                    config + ":down:kp",
                    config + ":down:kd"
                );
            } else {
                ctrl_ = std::make_shared<PIDACtrl>(
                    settings, 
                    config + ":up:kv",
                    config + ":up:ka",
                    config + ":up:kp",
                    config + ":up:kd"
                );
            }

            isDone_ = false;
            profile_->update(dist, 0, 0);
            startTime_ = subsystem.getRobot().getTime();
            startPosition_ = subsystem.getPosition();
            MessageLogger &logger = subsystem.getRobot().getMessageLogger() ;
            logger.startMessage(MessageLogger::MessageType::debug, subsystem.msg_id_);
            logger << "Motor/Encoder Target Distance (" ;
            logger << subsystem.getName() ;
            logger << "): " << dist;
            logger.endMessage();

            logger.startMessage(MessageLogger::MessageType::debug, subsystem.msg_id_);
            logger << "Motor/Encoder Velocity Profile: ("  ;
            logger << subsystem.getName() ;
            logger << ") : " << profile_->toString() ;
            logger.endMessage();

            subsystem.startPlot(plotid_, plot_columns_) ;
        }

        void MotorEncoderGoToAction::cancel() {
            isDone_ = true;
            // If we cancel a goto action, hold the motor at its current position
            getSubsystem().setDefaultAction(std::make_shared<MotorEncoderHoldAction>(getSubsystem()));
        }

        void MotorEncoderGoToAction::run() {
            if (isDone_) return;

            auto &subsystem = getSubsystem();
            auto &robot = subsystem.getRobot();

            double dt = robot.getDeltaTime();
            double elapsed = robot.getTime() - startTime_;
            double position = subsystem.getPosition();
            double traveled = normalizePosition(position - startPosition_);
            double remaining = normalizePosition(target_ - position);

            if (elapsed > profile_->getTotalTime()) {
                MessageLogger &logger = robot.getMessageLogger() ;
                logger.startMessage(MessageLogger::MessageType::debug, subsystem.getMsgID()) ;
                logger << "MotorEncoderGoToAction (" ;
                logger << subsystem.getName() ;
                logger << "): action completed" ;
                logger << ", remaining = " << remaining ;
                logger.endMessage() ;

                isDone_ = true ;
                subsystem.setMotor(0.0);
                subsystem.endPlot(plotid_) ;
                return;
            }

            double targetDist = profile_->getDistance(elapsed);
            double targetVel = profile_->getVelocity(elapsed);
            double targetAcc = profile_->getAccel(elapsed);

            double out = ctrl_->getOutput(
                targetAcc, targetVel, targetDist, traveled, dt
            );

            subsystem.setMotor(out);

            std::vector<double> data ;
            data.push_back(elapsed) ;
            data.push_back(startPosition_ + targetDist) ;
            data.push_back(position) ;
            data.push_back(targetVel) ;
            data.push_back(subsystem.getSpeedometer().getVelocity()) ;
            data.push_back(out) ;
            subsystem.addPlotData(plotid_, data) ;            
        }

        std::string MotorEncoderGoToAction::toString() {
            std::string str ;

            str = "MotorEncoderGoToAction " + std::to_string(target_) ;
            return str ;
        }
    }
}
