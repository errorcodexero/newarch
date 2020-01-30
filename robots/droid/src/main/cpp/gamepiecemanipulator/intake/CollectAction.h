#include "droidids.h"
#include <motorencodersubsystem/MotorEncoderPowerAction.h>

using namespace xero::misc;
using namespace xero::base;

namespace xero {
    namespace droid {
        class CollectAction: public SingleMotorPowerAction {   
        public :
            CollectAction(Intake &subsystem, bool isCollecting):
                SingleMotorPowerAction(subsystem, isCollecting ? "intake:collecting" : "intake:ejecting") {}

        } ;
    }
}
