#include "FireAction.h"
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
      
        FireAction::FireAction(Shooter &sub): xero::base::MotorEncoderVelocityAction(sub, 0), subsystem_(sub)
        {
            const std::string configName("shooter:velocity:ready_margin_percent");
            ready_margin_percent_ = getSubsystem().getRobot().getSettingsParser().getDouble(configName);
        }

        FireAction::~FireAction()
        {
        }

        void FireAction::run()
        {
            double target = 0 ;                                  //TODO: calculate target velocity
            setTarget(target);
            frc::SmartDashboard::PutNumber("tvel", getTarget());
            MotorEncoderVelocityAction::run();
            if (target != 0 && matchWithinPercentError(getTarget(), getSubsystem().getSpeedometer().getVelocity(), ready_margin_percent_)) {
                getSubsystem().setReadyToShoot(true);
            } else {
                getSubsystem().setReadyToShoot(false);
            }
        }

        void FireAction::cancel() {
            getSubsystem().setReadyToShoot(false);
        }
    }
}
