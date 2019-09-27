#pragma once

#include <oi/OIDevice.h> 
#include <Action.h>
#include <SequenceAction.h>

namespace xero {
    namespace swerve {
        class SwerveOISubsystem ;

        class SwerveOIDevice : public xero::base::OIDevice {
        public:
            SwerveOIDevice(SwerveOISubsystem &sub, int index) ;
            virtual ~SwerveOIDevice() ;

            virtual int getAutoModeSelector() {
                return getValue(automode_) ;
            }

            virtual void generateActions(xero::base::SequenceAction &seq) ;

            virtual void init() ;

        private:
            void bindOI() ;
            
        private:
            size_t automode_ ;
        } ;
    }
}
