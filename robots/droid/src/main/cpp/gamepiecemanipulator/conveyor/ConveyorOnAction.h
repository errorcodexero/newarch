#include "Conveyor.h"
#include <actions/Action.h>
#include <Robot.h>

namespace xero {
    namespace droid {
        class ConveyorOnAction : public xero::base::Action {
        public:
            ConveyorOnAction(Conveyor &sub): 
                xero::base::Action(sub.getRobot().getMessageLogger()),
                sub_(sub) {}

            void start() override 
            {
            }

            void run() override 
            { 
                sub_.setMotors(Conveyor::MotorState::MoveTowardsShooter); 
            }

            std::string toString() override { return "ConveyorOnAction"; }

        private:
            Conveyor &sub_;
        };
    }
}