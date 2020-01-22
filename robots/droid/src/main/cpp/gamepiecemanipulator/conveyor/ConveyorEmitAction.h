#include "ConveyorAction.h"

namespace xero {
    namespace droid {
        class ConveyorEmitAction : public ConveyorAction {
        public:
            ConveyorEmitAction(Conveyor &subsystem): ConveyorAction(subsystem) {}

            void start() override;
            void run() override;
        } ;
    }
}