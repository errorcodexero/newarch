#include "PhoenixGunnerGamepad.h"
#include "PhoenixOISubsystem.h"

using namespace xero::base ;

namespace xero {
    namespace phoenix {
        PhoenixGunnerGamepad::PhoenixGunnerGamepad(PhoenixOISubsystem &sub, int index) : HIDDevice(sub, index) {            
        }

        PhoenixGunnerGamepad::~PhoenixGunnerGamepad() {            
        }

        void PhoenixGunnerGamepad::computeState(ActionSequence &seq) {            
        }
    }
}