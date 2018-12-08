#pragma once

#include <OISubsystem.h>
#include "PhoenixOIDevice.h"
#include "PhoenixGunnerGamepad.h"
#include <DriverGamepad.h>
#include <memory>

namespace xero {
    namespace phoenix {
        class PhoenixOISubsystem : public xero::base::OISubsystem {
        public:
            PhoenixOISubsystem(xero::base::Robot &robot) ;
            virtual ~PhoenixOISubsystem() ;

        protected:
            void initializeWithGunner(int gunner, int driver) ;
            void initializeWithOI(int oi, int driver) ;

		private:
			bool detectOI(int index) ;
        } ;
    }
}
