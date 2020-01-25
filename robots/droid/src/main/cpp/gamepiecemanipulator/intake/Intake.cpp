#include "Intake.h"
#include "droidids.h"
#include "CollectAction.h"
#include <singlemotorsubsystem/SingleMotorSubsystem.h>

using namespace xero::misc;
using namespace xero::base;

namespace xero {
    namespace droid {
        Intake::Intake(Subsystem *parent): SingleMotorSubsystem(parent, "intake", MSG_GROUP_INTAKE) {

        }
    }
}
