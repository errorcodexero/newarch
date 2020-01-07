#include "collector/Collector.h"
#include "droidids.h"

using namespace xero::misc;
using namespace xero::base;

namespace xero {
    namespace droid {
        Collector::Collector(Subsystem *parent): SingleMotorSubsystem(parent, "collector", MSG_GROUP_COLLECTOR) {

        }
    }
}