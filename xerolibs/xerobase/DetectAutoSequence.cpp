#include "DetectAutoSequence.h"
#include "TeleopController.h"

namespace xero {
    namespace base {
        DetectAutoSequence::DetectAutoSequence(const std::string &name, std::shared_ptr<TeleopController> teleop, ActionPtr seq) {
            sequence_ = seq ;
            teleop_ = teleop ;
            name_ = name ;
        }

        DetectAutoSequence::~DetectAutoSequence() {            
        }

        void DetectAutoSequence::removeMe() {
            teleop_->removeDetector(this) ;
        }

        void DetectAutoSequence::printDetectors() {
            teleop_->printDetectors() ;
        }
    }
}