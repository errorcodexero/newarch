#include "Conveyor.h"
#include "ConveyorAction.h"
#include <actions/Action.h>
#include <Robot.h>

namespace xero {
    namespace droid {
        class ConveyorStopAction : public ConveyorAction {
        public:
            ConveyorStopAction(Conveyor &sub): ConveyorAction(sub)
            {                
            }

            virtual ~ConveyorStopAction()
            {                
            }

            void start() override {}
            void run() override { getSubsystem().setMotors(Conveyor::MotorState::Stopped); }

            std::string toString() override { return "ConveyorStopAction"; }

        private:
        };
    }
}