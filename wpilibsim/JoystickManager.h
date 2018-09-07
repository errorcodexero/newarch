#pragma once

#include <cstdlib>

namespace xero {
	namespace sim {
		class JoystickManager {
		public:
			JoystickManager() {				
			}

			virtual ~JoystickManager() {				
			}

			virtual size_t getCount() = 0 ;
			virtual size_t getButtonCount(size_t index) = 0 ;
			virtual bool getButtonValue(size_t index, size_t button) = 0 ;
			virtual size_t getAxisCount(size_t index) = 0 ;
			virtual double getAxisValue(size_t index, size_t axis) = 0 ;
			virtual size_t getPOVCount(size_t index) = 0 ;
			virtual int getPOVValue(size_t index, size_t pov) = 0 ;
			virtual void poll() = 0 ;
		} ;
	}
}
