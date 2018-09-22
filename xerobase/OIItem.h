#pragma once

#include <DriverStation.h>

namespace xero {
    namespace base {
		class OIItem {
		public:
			OIItem(int item) {
				item_ = item ;
			}

            virtual int getValue() const = 0 ;
			virtual void setValue(frc::DriverStation &ds, int index) = 0 ;
            virtual int getItem() const {
				return item_ ;
			}

		private:
			int item_ ;
		} ;
	}
}
