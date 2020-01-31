#include "Conveyor.h"
#include <actions/Action.h>
#include <Robot.h>

namespace xero {
    namespace droid {
        class ConveyorStopAction : public xero::base::Action {
        public:
            ConveyorStopAction(Conveyor &sub): 
                xero::base::Action(sub.getRobot().getMessageLogger()),
                sub_(sub) {}

            void start() override {}
            void run() override { sub_.setMotors(Conveyor::MotorState::Stopped); }

            std::string toString() override { return "ConveyorStopAction"; }

        private:
            Conveyor &sub_;
        };
    }
}