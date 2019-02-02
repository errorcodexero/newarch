#pragma once

#include <OIDevice.h>
#include <Action.h>

namespace xero {
    namespace phaser {
        class PhaserOISubsystem ;

        class PhaserOIDevice : public xero::base::OIDevice {
        public:
            PhaserOIDevice(PhaserOISubsystem &sub, int index) ;
            virtual ~PhaserOIDevice() ;

            virtual int getAutoModeSelector() {
                return getValue(automode_) ;
            }

            virtual void generateActions(xero::base::ActionSequence &seq) ;

        private:
            void createActions() ;
            void initialize() ;
            
        private:
            size_t automode_ ;
            size_t camera_switch_ ;
            size_t camera_mode_ ;   
        } ;
    }
}