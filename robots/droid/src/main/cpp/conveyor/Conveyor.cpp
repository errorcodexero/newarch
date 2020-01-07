#include "conveyor/Conveyor.h"
#include "droidids.h"

using namespace xero::misc;
using namespace xero::base;

namespace xero {
    namespace droid {
        Conveyor::Conveyor(Subsystem *parent): SingleMotorSubsystem(parent, "conveyor", MSG_GROUP_CONVEYOR) {
            
        }
    }
}