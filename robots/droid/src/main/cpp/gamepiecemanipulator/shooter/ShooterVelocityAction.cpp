#include "ShooterVelocityAction.h"
#include "Shooter.h"
#include <MessageLogger.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include <frc/shuffleboard/Shuffleboard.h>
#include <networktables/NetworkTableValue.h>
#include <wpi/StringMap.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include <string>
#include <xeromath.h>

using namespace xero::misc;
using namespace xero::base;

namespace xero {
    namespace droid {
      
        ShooterVelocityAction::ShooterVelocityAction(Shooter &sub): xero::base::MotorEncoderVelocityAction(sub, 0), subsystem_(sub)
        {
            const std::string configName("shooter:velocity:ready_margin_percent");
            ready_margin_percent_ = getSubsystem().getRobot().getSettingsParser().getDouble(configName);
        }

        ShooterVelocityAction::~ShooterVelocityAction()
        {
        }

        void ShooterVelocityAction::run()
        {
            double target = 0 ;                                  //TODO: calculate target velocity
            setTarget(target);
            frc::SmartDashboard::PutNumber("tvel", getTarget());
            MotorEncoderVelocityAction::run();
            if (target != 0 && xero::math::equalWithinPercentMargin(getTarget(), getSubsystem().getSpeedometer().getVelocity(), ready_margin_percent_)) {
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
