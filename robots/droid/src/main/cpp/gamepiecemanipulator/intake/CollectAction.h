#include "droidids.h"
#include <motorencodersubsystem/MotorEncoderPowerAction.h>

using namespace xero::misc;
using namespace xero::base;

namespace xero {
    namespace droid {
        class CollectAction: public SingleMotorPowerAction {   
            // action to go to a certain power... should be boolean for on/off.
            private :
            std::string collecting = "collecting" ;
            std::string ejecting = "ejecting" ;
            std::string intake_dur = "0.0" ;                              //hw:intake:duration ;

            public :
            CollectAction(Intake &subsystem, bool isCollecting):
                SingleMotorPowerAction(subsystem, isCollecting ? collecting : ejecting, intake_dur) {}

        } ;
    }
}
