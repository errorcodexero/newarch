#pragma once

#include <oi/OIDevice.h> 
#include <actions/Action.h>
#include <actions/SequenceAction.h>

namespace xero {
    namespace motorcontroller {
        class MotorControllerOISubsystem ;

        class MotorControllerOIDevice : public xero::base::OIDevice {
        public:
            MotorControllerOIDevice(MotorControllerOISubsystem &sub, int index) ;
            virtual ~MotorControllerOIDevice() ;

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
