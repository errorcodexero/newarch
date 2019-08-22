#pragma once

#include <oi/OIDevice.h> 
#include <Action.h>
#include <ActionSequence.h>

namespace xero {
    namespace swerve {
        class SwerveOISubsystem ;

        class SwerveOIDevice : public xero::base::OIDevice {
        public:
            SwerveOIDevice(SwerveOISubsystem &sub, int index) ;
            virtual ~SwerveOIDevice() ;

            virtual int getAutoModeSelector() {
                return 0 ;
            }

            virtual void generateActions(xero::base::ActionSequence &seq) {                
            }

        private:
        } ;
    }
}