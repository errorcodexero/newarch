#include "Arm.h"
#include "droidids.h"
#include "DeployAction.h"
#include <motorencodersubsystem/MotorEncoderSubsystem.h>

using namespace xero::misc;
using namespace xero::base;

namespace xero {
    namespace droid {
        Arm::Arm(Subsystem *parent): MotorEncoderSubsystem(parent, "arm", MSG_GROUP_ARM) {

        }
    }
}
