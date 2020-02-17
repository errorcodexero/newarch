#include "Shooter.h"
#include "droidids.h"
#include "ShooterAction.h"

using namespace xero::misc;
using namespace xero::base;

namespace xero {
    namespace droid {
        Shooter::Shooter(Subsystem *parent): MotorEncoderSubsystem(parent, "shooter", MSG_GROUP_SHOOTER) {
            setReadyToFire(false);

            auto &settings = getRobot().getSettingsParser();
            hoodServo_ = std::make_shared<frc::Servo>(settings.getInteger("hw:shooter:hood"));

            std::string hoodConfig = "shooter:hood:";
            hoodUpPos_ = settings.getDouble(hoodConfig + "up");
            hoodDownPos_ = settings.getDouble(hoodConfig + "down");
        }

        bool Shooter::canAcceptAction(xero::base::ActionPtr action) {
            return MotorEncoderSubsystem::canAcceptAction(action) || 
                std::dynamic_pointer_cast<ShooterAction>(action) != nullptr;
        }

        void Shooter::setHood(bool hood) {
            hoodServo_->Set(hood? hoodDownPos_ : hoodUpPos_);
        }
    }
}