#include "Conveyor.h"
#include "ConveyorAction.h"
#include <Robot.h>

namespace xero {
    namespace droid {
        class ConveyorOnAction : public ConveyorAction {
        public:
            ConveyorOnAction(Conveyor &sub): ConveyorAction(sub)
            {
            }

            void start() override 
            {
            }

            void run() override 
            { 
                getSubsystem().setMotors(Conveyor::MotorState::MoveTowardsShooter); 
            }

            std::string toString() override { return "ConveyorOnAction"; }

        private:
        };
    }
}