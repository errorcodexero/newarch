#pragma once

#include <oi/OISubsystem.h>
#include "DroidOIDevice.h"
#include <oi/DriverGamepad.h>
#include <memory>

namespace xero {
    namespace droid {
        class DroidOISubsystem : public xero::base::OISubsystem {
        public:
            DroidOISubsystem(xero::base::Subsystem *parent) ;
            virtual ~DroidOISubsystem() ;
        
        private:
        } ;
    }
}
