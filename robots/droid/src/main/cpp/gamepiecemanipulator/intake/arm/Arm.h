#include <motorencodersubsystem/MotorEncoderSubsystem.h>

namespace xero {
    namespace droid {
        class Arm : public xero::base::MotorEncoderSubsystem {
        public:
            Arm(xero::base::Subsystem *parent);
            virtual ~Arm() {}
        };
    }
}