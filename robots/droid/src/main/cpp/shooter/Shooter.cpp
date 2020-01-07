#include "shooter/Shooter.h"
#include "shooter/turret/Turret.h"
#include "droidids.h"

using namespace xero::misc;
using namespace xero::base;

namespace xero {
    namespace droid {
        Shooter::Shooter(Subsystem *parent): MotorEncoderSubsystem(parent, "shooter", MSG_GROUP_SHOOTER) {
            turret_ = std::make_shared<Turret>(this);
            addChild(turret_);
        }
    }
}