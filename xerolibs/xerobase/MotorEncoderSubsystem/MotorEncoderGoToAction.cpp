#include "MotorEncoderGoToAction.h"

#include <string>
#include <memory>
#include <cmath>

#include <Robot.h>
#include <MessageLogger.h>

#include "MotorEncoderSubsystem.h"

using namespace xero::misc ;

namespace xero {
    namespace base {
        MotorEncoderGoToAction::MotorEncoderGoToAction(MotorEncoderSubsystem &subsystem, double target):
            MotorEncoderSubsystemAction(subsystem) {
            
            std::string config = subsystem.configName_;
            auto &settings = subsystem.getRobot().getSettingsParser();
            
            target_ = target;
            threshold_ = settings.getDouble(config + ":threshold");
            
            profile_ = std::make_shared<TrapezoidalProfile>(
                settings.getDouble(config + ":maxv"),
                settings.getDouble(config + ":maxa"),
                settings.getDouble(config + ":maxd")
            );

            holdCtrl_.initFromSettingsExtended(settings, config + ":hold");
        }

        void MotorEncoderGoToAction::start() {
            MotorEncoderSubsystem &subsystem = getSubsystem();

            double dist = target_ - subsystem.getPosition();
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

        void MotorEncoderGoToAction::run() {
            MotorEncoderSubsystem &subsystem = getSubsystem();

            if (isDone_) {
                double dist = target_ - subsystem.getPosition();
                double out = holdCtrl_.getOutput(0, dist, subsystem.getRobot().getDeltaTime());
                subsystem.setMotor(out);
                return;
            }
            
            // TODO: finish
        }
    }
}