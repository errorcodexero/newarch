#pragma once

#include <oi/OISubsystem.h>
#include "SwerveOIDevice.h"
#include <oi/DriverGamepad.h>
#include <memory>

namespace xero {
    namespace swerve {
        class SwerveOISubsystem : public xero::base::OISubsystem {
        public:
            SwerveOISubsystem(xero::base::Robot &robot) ;
            virtual ~SwerveOISubsystem() ;
        
        protected:
            void initializeWithGunner(int gunner, int driver) ;
            void initializeWithOI(int oi, int driver) ;

        private:
            bool detectOI(int index) ;
        } ;
    }
}
