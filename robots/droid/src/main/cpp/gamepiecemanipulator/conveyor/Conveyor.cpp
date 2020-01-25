#include "Conveyor.h"
#include "droidids.h"

#include <singlemotorsubsystem/SingleMotorPowerAction.h>

using namespace xero::misc;
using namespace xero::base;

namespace xero {
    namespace droid {
        Conveyor::Conveyor(Subsystem *parent): SingleMotorSubsystem(parent, "conveyor", MSG_GROUP_CONVEYOR) {
            
        }

        void Conveyor::postHWInit() {
            setDefaultAction(std::make_shared<SingleMotorPowerAction>(*this, 0.0));
        }
    }
}