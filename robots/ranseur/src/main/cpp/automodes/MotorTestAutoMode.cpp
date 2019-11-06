#include "MotorTestAutoMode.h"
#include "Ranseur.h"
#include <actions/ParallelAction.h>
#include "ranseurrobotsubsystem/RanseurRobotSubsystem.h"

using namespace xero::base;

namespace xero {
    namespace ranseur {
        MotorTestAutoMode::MotorTestAutoMode(xero::base::Robot &robot):
            RanseurAutoModeBase(robot, "Motor Test", "Tests motors, duh") {
            
            auto &ranseur = dynamic_cast<Ranseur&>(getRobot());
            auto action = std::make_shared<ParallelAction>();
            for (auto motor : ranseur.getRanseurRobotSubsystem()->getMotors()) {
                action->addSubActionPair(motor, std::make_shared<SingleMotorPowerAction>(*motor, 1));
            }
            pushAction(action);
        }
    }
}