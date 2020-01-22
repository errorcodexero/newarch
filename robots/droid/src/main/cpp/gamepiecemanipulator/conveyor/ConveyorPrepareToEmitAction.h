#include "ConveyorAction.h"

namespace xero {
    namespace droid {
        class ConveyorPrepareToEmitAction : public ConveyorAction {
        public:
            ConveyorPrepareToEmitAction(Conveyor &subsystem): ConveyorAction(subsystem) {}

            void start() override;
            void run() override;
        } ;
    }
}