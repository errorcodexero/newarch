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
            armEncoder_ = std::make_shared<frc::Encoder>(
                settings.getInteger(arm + ":encoder:1"),
                 settings.getInteger(arm + ":encoder:2")
            );

            wristMotor_ = robot.getMotorFactory()->createMotor(wrist + ":motor");
            wristEncoder_ = std::make_shared<frc::Encoder>(
                settings.getInteger(arm + "wrist:encoder:1"),
                 settings.getInteger(arm + "wrist:encoder:2")
            );

            armPositionLowered_ = settings.getInteger(arm + ":arm:lowered");
            armPositionOffGround_ = settings.getInteger(arm + ":arm:off_ground");
            armPositionRaised_ = settings.getInteger(arm + ":arm:raised");

            wristPositionLowered_ = settings.getInteger(arm + ":wrist:lowered");
            wristPositionRaised_ = settings.getInteger(arm + ":wrist:raised");
        }

        TubArm::~TubArm() {            
        }

        void TubArm::computeState() {            
        }
    }
}