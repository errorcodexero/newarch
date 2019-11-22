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
        MotorEncoderGoToAction::MotorEncoderGoToAction(MotorEncoderSubsystem &subsystem, double target):
            MotorEncoderSubsystemAction(subsystem) {
            
            std::string config = subsystem.configName_ + ":goto";
            auto &settings = subsystem.getRobot().getSettingsParser();
            
            target_ = target;
            threshold_ = settings.getDouble(config + ":threshold");
            
            profile_ = std::make_shared<TrapezoidalProfile>(
                settings.getDouble(config + ":maxa"),
                settings.getDouble(config + ":maxd"),
                settings.getDouble(config + ":maxv")
            );
        }

        void MotorEncoderGoToAction::start() {
            MotorEncoderSubsystem &subsystem = getSubsystem();
            subsystem.setDefaultAction(std::make_shared<MotorEncoderHoldAction>(subsystem, target_));

            double dist = normalizePosition(target_ - subsystem.getPosition());
            if (std::fabs(dist) < threshold_) {
                isDone_ = true;
                MessageLogger &logger = subsystem.getRobot().getMessageLogger();
                logger.startMessage(MessageLogger::MessageType::debug, subsystem.msg_id_);
                logger << "MotorEncoderGoToHeightAction: action completed sucessfully in start";
                logger.endMessage();
                return;
            }

            std::string config = subsystem.configName_ + ":follower";
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
            logger << "Motor/Encoder Target Distance: " << dist;
            logger.endMessage();

            logger.startMessage(MessageLogger::MessageType::debug, subsystem.msg_id_);
            logger << "Motor/Encoder Velocity Profile: " << profile_->toString() ;
            logger.endMessage();
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
                logger << "MotorEncoderGoToHeightAction: action completed" ;
                logger << ", remaining = " << remaining ;
                logger.endMessage() ;

                isDone_ = true ;
                subsystem.setMotor(0.0);
                return;
            }

            double targetDist = profile_->getDistance(elapsed);
            double targetVel = profile_->getVelocity(elapsed);
            double targetAcc = profile_->getAccel(elapsed);

            double out = ctrl_->getOutput(
                targetAcc, targetVel, targetDist, traveled, dt
            );

            subsystem.setMotor(out);
        }

        std::string MotorEncoderGoToAction::toString() {
            std::string str ;

            str = "MotorEncoderGoToAction " + std::to_string(target_) ;
            return str ;
        }
    }
}
