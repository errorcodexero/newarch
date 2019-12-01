#pragma once

#include <oi/OISubsystem.h>
#include "$$RobotName$$OIDevice.h"
#include <oi/DriverGamepad.h>
#include <memory>

namespace xero {
    namespace $$robotname$$ {
        class $$RobotName$$OISubsystem : public xero::base::OISubsystem {
        public:
            $$RobotName$$OISubsystem(xero::base::Subsystem *parent) ;
            virtual ~$$RobotName$$OISubsystem() ;
        
        private:
        } ;
    }
}