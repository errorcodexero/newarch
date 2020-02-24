#include "Turret.h"
#include "droidids.h"

#include "TurretStopAction.h"
#include "FollowTargetAction.h"

using namespace xero::misc;
using namespace xero::base;

namespace xero {
    namespace droid {
        Turret::Turret(Subsystem *parent): MotorEncoderSubsystem(parent, "turret", MSG_GROUP_TURRET) {
#ifdef ALL_SMART_DASHBOARD            
            setSmartDashboardName("turret") ;
#endif            
            auto &settings = getRobot().getSettingsParser();
            minSafeAngle_ = settings.getDouble("turret:min");
            maxSafeAngle_ = settings.getDouble("turret:max");
            emergencyStop_ = false;
        }

        void Turret::computeState() {
            MotorEncoderSubsystem::computeState();
            
        }

        void Turret::postHWInit() {
            // setDefaultAction(std::make_shared<FollowTargetAction>(*this));
        }
        bool Turret::canAcceptAction(ActionPtr action) {
            return !emergencyStop_ && std::dynamic_pointer_cast<FollowTargetAction>(action) != nullptr;
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