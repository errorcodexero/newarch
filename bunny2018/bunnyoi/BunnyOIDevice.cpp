#include "BunnyOIDevice.h"
#include "BunnyOISubsystem.h"

using namespace xero::base ;

namespace xero {
    namespace bunny2018 {
        BunnyOIDevice::BunnyOIDevice(BunnyOISubsystem &sub, int index) : OIDevice(sub, index) {
            //
            // Auto modes
            //
            std::vector<double> mapping = { -0.9, -0.75, -0.5, -0.25, 0, 0.2, 0.4, 0.6, 0.8 } ;
            automode_ = mapAxisScale(6, mapping) ; 			
        }

        BunnyOIDevice::~BunnyOIDevice() {
        }
        
        void BunnyOIDevice::computeState(ActionSequence &seq) {
			OIDevice::computeState(seq) ;
        }
    }
}
