#include <motorencodersubsystem/MotorEncoderSubsystem.h>

namespace xero {
    namespace droid {
        class Turret;

        class Shooter : public xero::base::MotorEncoderSubsystem {
        public:
            Shooter(xero::base::Subsystem *parent);
            virtual ~Shooter() {}
        };
    }
}