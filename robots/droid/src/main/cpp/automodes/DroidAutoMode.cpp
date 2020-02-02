#include "DroidAutoMode.h"
#include "Droid.h"

#include "gamepiecemanipulator/GamePieceManipulator.h"
#include "gamepiecemanipulator/conveyor/ConveyorPrepareToReceiveAction.h"
#include "gamepiecemanipulator/conveyor/ConveyorReceiveAction.h"
#include "gamepiecemanipulator/conveyor/ConveyorPrepareToEmitAction.h"
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
        }

        DroidAutoMode::~DroidAutoMode()
        {            
        }
    }
}