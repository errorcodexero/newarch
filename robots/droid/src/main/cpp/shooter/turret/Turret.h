#include <motorencodersubsystem/MotorEncoderSubsystem.h>

namespace xero {
    namespace droid {
        class Turret : public xero::base::MotorEncoderSubsystem {
        public:
            Turret(xero::base::Subsystem *parent);
            virtual ~Turret() {}
        };
    }
}