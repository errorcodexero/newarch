#include "TubArm.h"
#include "ranseurids.h"
#include <Robot.h>
#include <actions/SequenceAction.h>
#include <frc/DigitalInput.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace ranseur {
      
        TubArm::TubArm(Subsystem *parent) : MotorEncoderSubsystem(parent, "tubarm", MSG_GROUP_TUBARM, true) {
            setSmartDashboardName("TubArm") ;
        }

        TubArm::~TubArm() {
        }

        void TubArm::init(LoopType lt) 
        {
            MotorEncoderSubsystem::init(lt) ;
        }

        void TubArm::postHWInit()
        {
            MotorEncoderSubsystem::postHWInit() ;
        }
    }
}
