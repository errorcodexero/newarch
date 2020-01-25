#include "ConveyorAction.h"

namespace xero {
    namespace droid {
        class ConveyorEmitAction : public ConveyorAction {
        public:
            ConveyorEmitAction(Conveyor &subsystem): ConveyorAction(subsystem) {}

            void start() override;
            void run() override;
        private:
            enum class State {
                Preparing,
                Emitting
            };
            State state_ = State::Preparing;
        } ;
    }
}