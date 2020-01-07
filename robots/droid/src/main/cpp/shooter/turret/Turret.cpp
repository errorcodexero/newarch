#include "shooter/turret/Turret.h"
#include "droidids.h"

using namespace xero::misc;
using namespace xero::base;

namespace xero {
    namespace droid {
        Turret::Turret(Subsystem *parent): MotorEncoderSubsystem(parent, "turret", MSG_GROUP_TURRET, true) {

        }
    }
}