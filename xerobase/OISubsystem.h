#pragma once

#include "Subsystem.h"
#include "DriverGamepad.h"
#include "ActionSequence.h"

namespace xero {
    namespace base {
        class OISubsystem : public Subsystem {
        public:
            OISubsystem(Robot &robot, const std::string &name, bool adddriver = true) ;
            virtual ~OISubsystem() ;

			void enable() {
				enabled_ = true ;
			}

			void disable() {
				enabled_ = false ;
			}            

            virtual void computeState()  ;
            virtual void run() ;

            int getAutoModeSelector() ;

protected:
            void addHIDDevice(std::shared_ptr<HIDDevice> dev) {
                hiddevices_.push_back(dev) ;
            }

        private:
            bool inited_ ;
            bool enabled_ ;
            std::shared_ptr<ActionSequence> seq_ ;
            std::list<std::shared_ptr<HIDDevice>> hiddevices_ ;
            std::shared_ptr<DriverGamepad> driver_ ;
        } ;
    }
}
