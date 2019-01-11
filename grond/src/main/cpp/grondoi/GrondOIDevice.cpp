#include "GrondOIDevice.h"
#include "GrondOISubsystem.h"
#include "Grond.h"
#include "Grondsubsystem/GrondSubsystem.h"
#include <ActionSequence.h>

using namespace xero::base ;

namespace xero {
    namespace grond {
        GrondOIDevice::GrondOIDevice(GrondOISubsystem &sub, int index) : OIDevice(sub, index) {
            initialize() ;
        }

        GrondOIDevice::~GrondOIDevice() {
        }
        
        void GrondOIDevice::initialize() {
        }

        
        int GrondOIDevice::getAutoModeSelector() {
            return -1 ;
        }

        void GrondOIDevice::createActions() {
        }

        void GrondOIDevice::generateActions(ActionSequence &seq) {
        }
    }
}
