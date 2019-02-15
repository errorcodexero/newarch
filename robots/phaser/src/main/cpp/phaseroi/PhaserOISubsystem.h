#pragma once

#include <OISubsystem.h>
#include "PhaserOIDevice.h"
#include <DriverGamepad.h>
#include <memory>

namespace xero {
    namespace phaser {
        class PhaserOISubsystem : public xero::base::OISubsystem {
        public:
            PhaserOISubsystem(xero::base::Robot &robot) ;
            virtual ~PhaserOISubsystem() ;
        
        protected:
            void initializeWithGunner(int gunner, int driver) ;
            void initializeWithOI(int oi, int driver) ;

        private:
            bool detectOI(int index) ;
        } ;
    }
}