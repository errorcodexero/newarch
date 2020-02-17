#include "FollowTargetAction.h"
#include "Droid.h"
#include "droidsubsystem/DroidSubsystem.h"
#include "targettracker/TargetTracker.h"
#include "limelight/DroidLimeLight.h"

using namespace xero::base;
using namespace xero::misc;

namespace xero {
    namespace droid {
        FollowTargetAction::FollowTargetAction(Turret &sub):
            MotorEncoderSubsystemAction(sub), 
            ll_(static_cast<Droid&>(sub.getRobot()).getDroidSubsystem()->getLimeLight()) {
            std::string thresholdConfig = "turret:fire_threshold";
            auto &settings = sub.getRobot().getSettingsParser();
            assert(settings.isDefined(thresholdConfig) && "turret:fire_threshold must be defined");
            threshold_ = settings.getDouble(thresholdConfig);
        }

        void FollowTargetAction::start() {
            MotorEncoderSubsystemAction::start();
            auto &sub = getSubsystem();
            pid_.initFromSettingsExtended(
                sub.getRobot().getSettingsParser(),
                "turret:follow",
                sub.isAngular()
            );

            ll_->setLedMode(LimeLight::ledMode::ForceOn) ;
        }

        void FollowTargetAction::run() {
            MotorEncoderSubsystemAction::run();
            auto &turret = getTurret();
            auto tracker = static_cast<Droid&>(getTurret().getRobot()).getDroidSubsystem()->getTargetTracker();
            auto &logger = turret.getRobot().getMessageLogger() ;
            
            double error = tracker->getRelativeAngle();
            
            // Check for limits.
            double absTarget = turret.getPosition() - error;

            double out = pid_.getOutput(error, 0, turret.getRobot().getDeltaTime()) ; 
            if (abs(out) > 0.007 && abs(out) < 0.05) {
                if (out > 0) out = 0.05;
                else out = -0.05;
            }
            turret.setMotorPower(out) ;

            logger.startMessage(MessageLogger::MessageType::debug, turret.getMsgID()) ;
            logger << "FollowTargetAction (" << turret.getName() << "):" ;
            logger << " abscurrent " << turret.getPosition();
            logger << " abstarget " << absTarget;
            logger << " error " << error;
            logger << " output " << out ;
            logger.endMessage() ;

            turret.readyToFire_ = (abs(error) < threshold_);
        }

        void FollowTargetAction::cancel() {
            MotorEncoderSubsystemAction::cancel();
            setDone();
            getTurret().setMotor(0.0);
            ll_->setLedMode(LimeLight::ledMode::ForceOff) ;            
        }
    }
}
