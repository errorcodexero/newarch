#pragma once

#ifdef __CYGWIN__

#include <dinput.h>
#include "JoystickManager.h"
#include <vector>

namespace xero {
	namespace sim {
		class DIJoystickManager: public JoystickManager {
		public:
			DIJoystickManager() ;
			virtual ~DIJoystickManager() ;

			virtual size_t getCount();
			virtual size_t getButtonCount(size_t index);
			virtual bool getButtonValue(size_t index, size_t button);
			virtual size_t getAxisCount(size_t index);
			virtual double getAxisValue(size_t index, size_t axis);
			virtual size_t getPOVCount(size_t index);
			virtual int getPOVValue(size_t index, size_t pov);
			virtual void poll() ;	

			bool enumCallback(const DIDEVICEINSTANCE *inst) ;

		private:
			void init() ;
			double scaleAxisValue(LONG v) ;

		private:
			LPDIRECTINPUT lpDinput_ ;
			std::vector<LPDIRECTINPUTDEVICE8> devices_ ;
			std::vector<DIDEVCAPS> caps_ ;
			std::vector<DIJOYSTATE2> state_ ;
		} ;
	}
}

#endif

