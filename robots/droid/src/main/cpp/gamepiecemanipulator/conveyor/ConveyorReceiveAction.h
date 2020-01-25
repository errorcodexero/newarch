#include "ConveyorAction.h"

namespace xero {
    namespace droid {
        class ConveyorReceiveAction : public ConveyorAction {
        public:
            ConveyorReceiveAction(Conveyor &subsystem): ConveyorAction(subsystem) {}

            void start() override;
            void run() override;

        private:
            enum class State {
                Preparing,
                Ready,
                StartingReceive,
                FinishingReceive
            };
            State state_ = State::Preparing;
        } ;
    }
}