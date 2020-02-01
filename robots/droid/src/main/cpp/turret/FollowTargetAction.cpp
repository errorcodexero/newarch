#include "FollowTargetAction.h"

namespace xero {
    namespace droid {
        FollowTargetAction::FollowTargetAction(Turret &subsystem):
            MotorEncoderHoldAction(subsystem, 0) {
            
            std::string thresholdConfig = "turret:fire_threshold";
            auto &settings = getSubsystem().getRobot().getSettingsParser();
            assert(settings.isDefined(thresholdConfig) && "turret:fire_threshold must be defined");
            threshold_ = settings.getDouble(thresholdConfig);
        }

        void FollowTargetAction::run() {
            double target = 0.0 ;          ///get target from limelight
            setTarget(target) ;
            MotorEncoderHoldAction::run() ;
            
            auto &turret = getTurret();
            double error = abs(turret.getPosition());
            turret.readyToFire_ = (error < threshold_);
        }
    }
}
