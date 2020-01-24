#include <singlemotorsubsystem/SingleMotorSubsystem.h>

namespace xero {
    namespace droid {
        class Intake : public xero::base::SingleMotorSubsystem {
        public:
            Intake(xero::base::Subsystem *parent);
            virtual ~Intake() {}
        };
    }
}