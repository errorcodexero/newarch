#pragma once

#include <oi/OISubsystem.h>
#include "droidOIDevice.h"
#include <oi/DriverGamepad.h>
#include <memory>

namespace xero {
    namespace droid {
        class droidOISubsystem : public xero::base::OISubsystem {
        public:
            droidOISubsystem(xero::base::Subsystem *parent) ;
            virtual ~droidOISubsystem() ;
        
        private:
        } ;
    }
}
