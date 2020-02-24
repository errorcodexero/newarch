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
      
        ShooterVelocityAction::ShooterVelocityAction(Shooter &sub, double target, Shooter::HoodPosition hood): 
            xero::base::MotorEncoderVelocityAction(sub, 0), subsystem_(sub), hood_(hood)
        {
            const std::string configName("shooter:velocity:ready_margin_percent");
            ready_margin_percent_ = getSubsystem().getRobot().getSettingsParser().getDouble(configName);
            setTarget(target) ;
        }

        ShooterVelocityAction::~ShooterVelocityAction()
        {
        }

        void ShooterVelocityAction::run()
        {
            getSubsystem().setHood(hood_);
            frc::SmartDashboard::PutNumber("tvel", getTarget());
            MotorEncoderVelocityAction::run();
            updateReadyToFire();
        }

        void ShooterVelocityAction::cancel() {
            MotorEncoderSubsystemAction::cancel();
            getSubsystem().setReadyToFire(false);
            setDone();
        }

        void ShooterVelocityAction::setTarget(double target) {
            MotorEncoderVelocityAction::setTarget(target);
            updateReadyToFire();
        }

        void ShooterVelocityAction::updateReadyToFire() {
            if (getTarget() != 0 && xero::math::equalWithinPercentMargin(getTarget(), getSubsystem().getSpeedometer().getVelocity(), ready_margin_percent_)) {
                getSubsystem().setReadyToFire(true);
            } else {
                getSubsystem().setReadyToFire(false);
            }
        }
    }
}
