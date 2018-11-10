#pragma once

#include <OIDevice.h>
#include <Action.h>

namespace xero {
    namespace bunny2018 {
        class BunnyOISubsystem ;

        class BunnyOIDevice : public xero::base::OIDevice {
		public:
            BunnyOIDevice(BunnyOISubsystem &, int index) ;
            virtual ~BunnyOIDevice() ;

            virtual int getAutoModeSelector() {
                return getValue(automode_) ;
            }

            virtual void computeState(xero::base::ActionSequence &seq) ;

		private:
            size_t automode_ ;		
		} ;
	}
}
