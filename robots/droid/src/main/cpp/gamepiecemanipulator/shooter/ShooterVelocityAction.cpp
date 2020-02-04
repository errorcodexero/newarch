#include "ShooterVelocityAction.h"
#include "Shooter.h"
#include <MessageLogger.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include <frc/shuffleboard/Shuffleboard.h>
#include <networktables/NetworkTableValue.h>
#include <wpi/StringMap.h>
#include <cmath>
#include <frc/smartdashboard/SmartDashboard.h>
#include <string>

using namespace xero::misc;
using namespace xero::base;

namespace xero {
    namespace droid {

        static bool matchWithinPercentError(double target, double measured, double percentage) {
            assert(target != 0);
            return (std::fabs(100.0 * (measured - target)/target) <= percentage);
        }
      
        ShooterVelocityAction::ShooterVelocityAction(Shooter &sub, double target): xero::base::MotorEncoderVelocityAction(sub, 0), subsystem_(sub)
        {
            target_ = target ;
            const std::string configName("shooter:velocity:ready_margin_percent");
            ready_margin_percent_ = getSubsystem().getRobot().getSettingsParser().getDouble(configName);
        }

        ShooterVelocityAction::~ShooterVelocityAction()
        {
        }

        void ShooterVelocityAction::run()
        {
            setTarget(target_);
            frc::SmartDashboard::PutNumber("tvel", target_);

            MotorEncoderVelocityAction::run();

            if (target_ != 0 && matchWithinPercentError(getTarget(), getSubsystem().getSpeedometer().getVelocity(), ready_margin_percent_)) {
                getSubsystem().setReadyToShoot(true);
            } else {
                getSubsystem().setReadyToShoot(false);
            }
        }

        void ShooterVelocityAction::cancel() {
            getSubsystem().setReadyToShoot(false);
        }
    }
}
