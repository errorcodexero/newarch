#pragma once

#include <oi/OISubsystem.h>
#include "MotorControllerOIDevice.h"
#include <oi/DriverGamepad.h>
#include <memory>

namespace xero {
    namespace motorcontroller {
        class MotorControllerOISubsystem : public xero::base::OISubsystem {
        public:
            MotorControllerOISubsystem(xero::base::Subsystem *parent) ;
            virtual ~MotorControllerOISubsystem() ;
        
        private:
        } ;
    }
}
