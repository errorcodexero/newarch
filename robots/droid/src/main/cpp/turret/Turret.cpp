#include "Turret.h"
#include "droidids.h"

#include "TurretStopAction.h"
#include "FollowTargetAction.h"

using namespace xero::misc;
using namespace xero::base;

namespace xero {
    namespace droid {
        Turret::Turret(Subsystem *parent): MotorEncoderSubsystem(parent, "turret", MSG_GROUP_TURRET) {

        }

        
        void Turret::postHWInit() {
            setDefaultAction(std::make_shared<FollowTargetAction>(*this));
        }
        bool Turret::canAcceptAction(ActionPtr action) {
            return std::dynamic_pointer_cast<FollowTargetAction>(action) != nullptr;
        }
        bool Turret::canAcceptDefaultAction(ActionPtr action) {
            return std::dynamic_pointer_cast<TurretStopAction>(action) != nullptr;
        }
    }
}