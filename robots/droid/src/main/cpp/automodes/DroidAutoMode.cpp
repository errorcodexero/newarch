#include "DroidAutoMode.h"
#include "Droid.h"

#include "gamepiecemanipulator/GamePieceManipulator.h"
#include "gamepiecemanipulator/conveyor/ConveyorPrepareToReceiveAction.h"
#include "gamepiecemanipulator/conveyor/ConveyorReceiveAction.h"
#include "gamepiecemanipulator/conveyor/ConveyorPrepareToEmitAction.h"
#include "gamepiecemanipulator/conveyor/ConveyorEmitAction.h"

#include "gamepiecemanipulator/shooter/Shooter.h"
#include "gamepiecemanipulator/shooter/ShooterVelocityAction.h"

#include "turret/FollowTargetAction.h"

namespace xero
{
    namespace droid
    {
        DroidAutoMode::DroidAutoMode(xero::base::Robot &robot, const std::string &name, const std::string &desc, 
                                     const std::string &configName) : AutoMode(robot, name, desc)
        {
            auto &settings = robot.getSettingsParser();
            std::string configBase = "auto:" + configName + "initial_pose:";
            startX_ = settings.getDouble(configBase + "x");
            startY_ = settings.getDouble(configBase + "y");
            startAngle_ = settings.getDouble(configBase + "angle");
        }

        DroidAutoMode::~DroidAutoMode()
        {            
        }

        void DroidAutoMode::start() {
            auto db = getDroidSubsystem()->getTankDrive();
            db->getKinematics()->set(startX_, startY_, startAngle_);
            db->setAngle(startAngle_);
        }

        std::shared_ptr<DroidSubsystem> DroidAutoMode::getDroidSubsystem() {
            return static_cast<Droid&>(getRobot()).getDroidSubsystem();
        }
    }
}
