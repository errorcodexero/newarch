#include "OIDevice.h"
#include <DriverStation.h>

namespace xero {
    namespace base {
        OIDevice::OIDevice(Subsystem &sub, int index) : HIDDevice(sub, index) {
        }

        OIDevice::~OIDevice() {            
        }

        void OIDevice::computeState(ActionSequence &seq) {
            frc::DriverStation &ds = frc::DriverStation::GetInstance() ;

			for(auto item : items_)
				item->setValue(ds, getIndex()) ;
        }

        size_t OIDevice::mapButton(int button, OIButton::ButtonType btype) {
            auto feature_p  = std::make_shared<OIButton>(button, btype) ;
			items_.push_back(feature_p) ;
			return items_.size() - 1 ;
        }

        size_t OIDevice::mapAxisScale(int axis, const std::vector<double> &mapping) {
            auto feature_p = std::make_shared<OIAxisScale>(axis, mapping) ;
			items_.push_back(feature_p) ;
			return items_.size() - 1 ;
        }        
        
        size_t OIDevice::mapAxisSwitch(int axis, int positions) {
            auto feature_p = std::make_shared<OIAxisSwitch>(axis, positions) ;
			items_.push_back(feature_p) ;
			return items_.size() - 1 ;
        }

        int OIDevice::getValue(size_t which) {
			if (which < items_.size())
				return items_[which]->getValue() ;

			return 0 ;
        }        
    }
}