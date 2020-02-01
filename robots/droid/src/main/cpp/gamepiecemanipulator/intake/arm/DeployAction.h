#include "droidids.h"
#include <motorencodersubsystem/MotorEncoderGoToAction.h>

using namespace xero::misc;
using namespace xero::base;

namespace xero {
    namespace droid {
        class DeployAction: public MotorEncoderGoToAction {   
            // action to go to a certain angle... should be boolean for deployed/ not deployed.
           public :
            DeployAction(Arm &subsystem, bool isDeployed): 
                MotorEncoderGoToAction(subsystem, isDeployed ? "deploy" : "retract", "intake:arm") {}

        } ;
    }
}
