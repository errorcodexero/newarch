#include "ConveyorAction.h"

namespace xero {
    namespace droid {
        class ConveyorPrepareToReceiveAction : public ConveyorAction {
        public:
            ConveyorPrepareToReceiveAction(Conveyor &subsystem): ConveyorAction(subsystem) {}

            void start() override;
            void run() override;
        } ;
    }
}