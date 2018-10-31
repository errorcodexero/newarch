#pragma once

#include "Subsystem.h"

namespace xero {
    namespace base {
        class ActionSequence ;

        class HIDDevice  {
        public:
            HIDDevice(Subsystem &sub, int index) : subsystem_(sub) {
                index_ = index ;
            }

            virtual ~HIDDevice() {                
            }

            Subsystem &getSubsystem() {
                return subsystem_ ;
            }

            int getIndex() const {
                return index_ ;
            }

            virtual void computeState(ActionSequence &) = 0 ;

            virtual int getAutoModeSelector() {
                return -1 ;
            }

        private:
            Subsystem &subsystem_ ;
            int index_ ;
        } ;
    }
}