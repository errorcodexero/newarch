#include "Turret.h"
#include "droidids.h"

#include "TurretStopAction.h"
#include "FollowTargetAction.h"

using namespace xero::misc;
using namespace xero::base;

namespace xero {
    namespace droid {
        Turret::Turret(Subsystem *parent): MotorEncoderSubsystem(parent, "turret", MSG_GROUP_TURRET) {
            setSmartDashboardName("turret") ;
            auto &settings = getRobot().getSettingsParser();
            minSafeAngle_ = settings.getDouble("turret:min");
            maxSafeAngle_ = settings.getDouble("turret:max");
        }

        void Turret::postHWInit() {
            // setDefaultAction(std::make_shared<FollowTargetAction>(*this));
        }
        bool Turret::canAcceptAction(ActionPtr action) {
            return std::dynamic_pointer_cast<FollowTargetAction>(action) != nullptr;
        }
        bool Turret::canAcceptDefaultAction(ActionPtr action) {
            return std::dynamic_pointer_cast<TurretStopAction>(action) != nullptr;
        }

        void Turret::setMotorPower(double power) {
            double pos = getPosition();
            if (power < 0 && pos <= getMinSafeAngle()) setMotor(0);
            else if (power > 0 && pos >= getMaxSafeAngle()) setMotor(0);
            else setMotor(power);
        }
    }
}