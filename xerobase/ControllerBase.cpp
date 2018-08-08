#include "ControllerBase.h"

namespace xero {
    namespace base {
        ControllerBase::ControllerBase(Robot &robot) : robot_(robot) {
        }

        ControllerBase::~ControllerBase() {            
        }
    }
}