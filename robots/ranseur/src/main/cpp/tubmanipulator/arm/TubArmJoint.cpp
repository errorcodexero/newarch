#include "TubArmJoint.h"
#include <Robot.h>

namespace xero {
    namespace ranseur {
        TubArmJoint::TubArmJoint(Robot &robot, 
                                const std::string &name, 
                                const std::string &configName,
                                const std::vector<std::string> positions, 
                                uint64_t id):
            SingleMotorSubsystem(robot, name, configName + ":motor", id) {

            auto &settings = robot.getSettingsParser();
            std::transform(positions.begin(), positions.end(), 
                           std::inserter(positions_, positions_.end()),
                           [settings](std::string name) -> std::pair<std::string, int> {
                return std::make_pair<std::string, int>()
            })
        }
    }
}