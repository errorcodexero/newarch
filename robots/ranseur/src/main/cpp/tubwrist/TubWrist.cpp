#include "TubWrist.h"
#include "ranseurids.h"
#include <Robot.h>
#include <actions/SequenceAction.h>
#include <frc/DigitalInput.h>
#include <motorencodersubsystem/MotorEncoderSubsystem.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace ranseur {
      
    TubWrist::TubWrist(Subsystem *parent) : MotorEncoderSubsystem(parent, "tubwrist", MSG_GROUP_TUBWRIST, true) {
            setSmartDashboardName("TubWrist") ;
        }

        TubWrist::~TubWrist() {
        }
    }
}
