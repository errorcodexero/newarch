#include "Shooter.h"
#include "droidids.h"

using namespace xero::misc;
using namespace xero::base;

namespace xero {
    namespace droid {
        Shooter::Shooter(Subsystem *parent): MotorEncoderSubsystem(parent, "shooter", MSG_GROUP_SHOOTER) {
            setReadyToFire(false);
        }
    }
}