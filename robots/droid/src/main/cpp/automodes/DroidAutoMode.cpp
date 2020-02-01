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
            auto droid = dynamic_cast<Droid&>(robot).getDroidSubsystem();
            auto conveyor = droid->getGamePieceManipulator()->getConveyor();
            auto turret = droid->getTurret();
            auto shooter = droid->getGamePieceManipulator()->getShooter();

            // continually follow the target
            //pushSubActionPair(turret, std::make_shared<FollowTargetAction>(*turret), false);

            // pick up a ball
            pushSubActionPair(conveyor, std::make_shared<ConveyorPrepareToReceiveAction>(*conveyor));
            pushSubActionPair(conveyor, std::make_shared<ConveyorReceiveAction>(*conveyor));

            // spin up the shooter, prepare to fire
            pushSubActionPair(shooter, std::make_shared<FireAction>(*shooter), false);
            pushSubActionPair(conveyor, std::make_shared<ConveyorPrepareToEmitAction>(*conveyor));
            
            // fire
            pushSubActionPair(conveyor, std::make_shared<ConveyorEmitAction>(*conveyor));
        }

        DroidAutoMode::~DroidAutoMode()
        {            
        }
    }
}