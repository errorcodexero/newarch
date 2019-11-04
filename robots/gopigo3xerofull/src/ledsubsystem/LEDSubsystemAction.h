#pragma once

#include <Action.h>

namespace xero
{
    namespace gopigo3
    {
        class LEDSubsystem ;
        
        class LEDSubsystemAction : public xero::base::Action
        {
        public:
          LEDSubsystemAction(LEDSubsystem &subsystem) : subsystem_(subsystem) {
          }

          ~LEDSubsystemAction() {
          }

        protected:
            LEDSubsystem &getSubsystem() {
                return subsystem_;
            }

        private: 
            LEDSubsystem &subsystem_;
        };
    } // namespace gopigo3
} // namespace xero
