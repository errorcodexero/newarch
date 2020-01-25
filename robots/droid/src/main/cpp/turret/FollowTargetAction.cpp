#include "FollowTargetAction.h"

namespace xero {
    namespace droid {
        void FollowTargetAction::run() {
          
            double target = 0.0 ;          ///get target from limelight
            setTarget(target) ;
            MotorEncoderGoToAction::run() ;

        }
    }
}
