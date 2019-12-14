#pragma once

#include <oi/OISubsystem.h>
#include "PhoenixOIDevice.h"
#include <oi/DriverGamepad.h>
#include <memory>

namespace xero {
    namespace phoenix {
        class PhoenixOISubsystem : public xero::base::OISubsystem {
        public:
            PhoenixOISubsystem(xero::base::Subsystem *parent) ;
            virtual ~PhoenixOISubsystem() ;

        protected:
            void initializeWithGunner(int gunner, int driver) ;
            void initializeWithOI(int oi, int driver) ;

        private:
            bool detectOI(int index) ;
        } ;
    }
}
