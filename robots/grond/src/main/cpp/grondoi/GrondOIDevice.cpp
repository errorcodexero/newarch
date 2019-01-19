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
            std::vector<double> mapping = { -0.9, -0.75, -0.5, -0.25, 0, 0.2, 0.4, 0.6, 0.8, 1.0 } ;
            automode_ = mapAxisScale(6, mapping) ;             
        }

        
        void GrondOIDevice::createActions() {
        }

        void GrondOIDevice::generateActions(ActionSequence &seq) {
        }
    }
}
