#pragma once

#include "HIDDevice.h"
#include "OIButton.h"
#include "OIAxisScale.h"
#include "OIAxisSwitch.h"
#include <map>
#include <memory>

namespace xero {
    namespace base {
        class OIDevice : public HIDDevice { 
        public:
            OIDevice(Subsystem &sub, int index) ;
            virtual ~OIDevice() ;

            virtual void computeState(ActionSequence &seq) ;
            
        protected:
            size_t mapButton(int button, OIButton::ButtonType byte) ;
        	size_t mapAxisScale(int axis, const std::vector<double> &mapping) ;
            size_t mapAxisSwitch(int axis, int positions) ;
            int getValue(size_t which) ;  

        private:
            std::vector<std::shared_ptr<OIItem>> items_ ;
        } ;
    }
}