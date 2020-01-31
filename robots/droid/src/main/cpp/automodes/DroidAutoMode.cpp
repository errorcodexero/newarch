#include "DroidAutoMode.h"
#include "Droid.h"

#include "gamepiecemanipulator/GamePieceManipulator.h"
#include "gamepiecemanipulator/conveyor/ConveyorPrepareToReceiveAction.h"
#include "gamepiecemanipulator/conveyor/ConveyorReceiveAction.h"
#include "gamepiecemanipulator/conveyor/ConveyorPrepareToEmitAction.h"
#include "gamepiecemanipulator/conveyor/ConveyorEmitAction.h"

namespace xero
{
    namespace droid
    {
        DroidAutoMode::DroidAutoMode(xero::base::Robot &robot, const std::string &name, const std::string &desc) : AutoMode(robot, name, desc)
        {
            auto &droid = dynamic_cast<Droid&>(robot);
            auto conveyor = droid.getDroidSubsystem()->getGamePieceManipulator()->getConveyor();

            pushSubActionPair(conveyor, std::make_shared<ConveyorPrepareToReceiveAction>(*conveyor));
            pushSubActionPair(conveyor, std::make_shared<ConveyorReceiveAction>(*conveyor));
            pushSubActionPair(conveyor, std::make_shared<ConveyorPrepareToEmitAction>(*conveyor));
            pushSubActionPair(conveyor, std::make_shared<ConveyorEmitAction>(*conveyor));
        }

        DroidAutoMode::~DroidAutoMode()
        {            
        }
    }
}