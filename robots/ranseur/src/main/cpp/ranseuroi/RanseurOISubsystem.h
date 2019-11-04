#pragma once

#include <oi/OISubsystem.h>
#include "RanseurOIDevice.h"
#include <oi/DriverGamepad.h>
#include <memory>

namespace xero {
    namespace ranseur {
        class RanseurOISubsystem : public xero::base::OISubsystem {
        public:
            RanseurOISubsystem(xero::base::Subsystem *parent) ;
            virtual ~RanseurOISubsystem() ;
        
        protected:
            void initializeWithGunner(int gunner, int driver) ;
            void initializeWithOI(int oi, int driver) ;
        } ;
    }
}