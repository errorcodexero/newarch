#include <motors/MotorFactory.h>
#include <Robot.h>
#include "TubArm.h"

using namespace xero::base;

namespace xero {
    namespace ranseur {
        TubArm::TubArm(Robot &robot, std::string configID): Subsystem(robot, "tubarm") {
            std::string arm = configID + ":arm";
            std::string wrist = configID + ":wrist";

            auto &settings = robot.getSettingsParser();

            armMotor_ = robot.getMotorFactory()->createMotor(arm + ":motor");
            armEncoder_ = frc::Encoder(
                settings.getInteger(arm + ":encoder:1"),
                 settings.getInteger(arm + ":encoder:2")
            );

            wristMotor_ = robot.getMotorFactory()->createMotor(wrist + ":motor");
            wristEncoder_ = frc::Encoder(
                settings.getInteger(arm + "wrist:encoder:1"),
                 settings.getInteger(arm + "wrist:encoder:2")
            );
        }
    }
}