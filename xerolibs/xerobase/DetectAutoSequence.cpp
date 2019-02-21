#include "DetectAutoSequence.h"
#include "TeleopController.h"

namespace xero {
    namespace base {
        void DetectAutoSequence::removeMe() {
            teleop_->removeDetector(this) ;
        }
    }
}