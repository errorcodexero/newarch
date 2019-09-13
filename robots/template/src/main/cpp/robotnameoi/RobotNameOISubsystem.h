#pragma once

#include <oi/OISubsystem.h>
#include "$$RobotName$$OIDevice.h"
#include <oi/DriverGamepad.h>
#include <memory>

namespace xero {
    namespace $$robotname$$ {
        class $$RobotName$$OISubsystem : public xero::base::OISubsystem {
        public:
            $$RobotName$$OISubsystem(xero::base::Robot &robot) ;
            virtual ~$$RobotName$$OISubsystem() ;
        
        protected:
            void initializeWithGunner(int gunner, int driver) ;
            void initializeWithOI(int oi, int driver) ;

        private:
            bool detectOI(int index) ;
        } ;
    }
}