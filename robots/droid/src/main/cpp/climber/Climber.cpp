#include "climber/Climber.h"
#include "droidids.h"

using namespace xero::misc;
using namespace xero::base;

namespace xero {
    namespace droid {
        Climber::Climber(Subsystem *parent): Subsystem(parent, "collector") {
            msg_id_ = MSG_GROUP_CLIMBER;
        }
    }
}