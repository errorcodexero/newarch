#include "DroidAutoMode.h"
#include "Droid.h"

#include "gamepiecemanipulator/GamePieceManipulator.h"
#include "gamepiecemanipulator/conveyor/ConveyorEmitAction.h"

#include "gamepiecemanipulator/shooter/Shooter.h"
#include "gamepiecemanipulator/shooter/FireAction.h"

#include "turret/FollowTargetAction.h"

namespace xero
{
    namespace droid
    {
        DroidAutoMode::DroidAutoMode(xero::base::Robot &robot, const std::string &name, const std::string &desc) : AutoMode(robot, name, desc)
        {
            auto droid = dynamic_cast<Droid&>(robot).getDroidSubsystem();
            auto conveyor = droid->getGamePieceManipulator()->getConveyor();
            auto turret = droid->getTurret();
            auto shooter = droid->getGamePieceManipulator()->getShooter();
        }

        DroidAutoMode::~DroidAutoMode()
        {            
        }
    }
}