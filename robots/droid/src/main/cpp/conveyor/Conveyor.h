#include <singlemotorsubsystem/SingleMotorSubsystem.h>

namespace xero {
    namespace droid {
        class Conveyor : public xero::base::SingleMotorSubsystem {
        public:
            Conveyor(xero::base::Subsystem *parent);
            virtual ~Conveyor() {}
        };
    }
}