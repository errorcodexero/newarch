#include <actions/Action.h>

namespace xero {
    namespace droid {
        class Conveyor;
        class ConveyorAction : public xero::base::Action {
        public:
            ConveyorAction(Conveyor &subsystem);

            Conveyor &getSubsystem() {
                return subsystem_ ;
            }

        private:
            Conveyor &subsystem_ ;
        } ;
    }
}