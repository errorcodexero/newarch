#include <motorencodersubsystem/MotorEncoderSubsystem.h>

namespace xero {
    namespace droid {
        class ControlPanelRotator : public xero::base::MotorEncoderSubsystem {
        public:
            ControlPanelRotator(xero::base::Subsystem *parent);
            virtual ~ControlPanelRotator() {}
        };
    }
}