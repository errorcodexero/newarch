#include <motors/MotorFactory.h>
#include <Robot.h>
#include "TubArm.h"

using namespace xero::base;

namespace xero {
    namespace ranseur {
        TubArm::TubArm(Subsystem *parent, std::string configID): Subsystem(parent, "tubarm") {
            std::string arm = configID + ":arm";
            std::string wrist = configID + ":wrist";
            
            auto &robot = getRobot();
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
        }

        TubArm::~TubArm() {            
        }

        void TubArm::computeState() {            
        }
    }
}