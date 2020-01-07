#include <singlemotorsubsystem/SingleMotorSubsystem.h>

namespace xero {
    namespace droid {
        class Collector : public xero::base::SingleMotorSubsystem {
        public:
            Collector(xero::base::Subsystem *parent);
            virtual ~Collector() {}
        };
    }
}