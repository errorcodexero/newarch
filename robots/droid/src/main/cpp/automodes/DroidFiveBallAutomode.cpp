#include "DroidFiveBallAutomode.h"
#include "droidsubsystem/DroidSubsystem.h"
#include "gamepiecemanipulator/FireAction.h"
#include "gamepiecemanipulator/StartCollectAction.h"
#include "gamepiecemanipulator/StopCollectAction.h"

#include <actions/ParallelAction.h>
#include <tankdrive/actions/TankDriveFollowPathAction.h>

using namespace xero::base;
using namespace xero::misc;
namespace xero
{
    namespace droid
    {
        DroidFiveBallAutomode::DroidFiveBallAutomode(xero::base::Robot &robot) : 
            DroidAutoMode(robot, "FarSideFive", "Start on far side, collect two, score five")
        {
            // Turret will track target as its default action
            // TODO: assign initial position to kinematic model?

            auto droid = getDroidSubsystem();
            auto db = droid->getTankDrive();
            auto manip = droid->getGamePieceManipulator();

            // In parallel...
            auto collectPar = std::make_shared<ParallelAction>(robot.getMessageLogger());
            pushAction(collectPar);

            //     Drive to collect five balls
            collectPar->addSubActionPair(db, std::make_shared<TankDriveFollowPathAction>(*db, "five_ball_auto_collect"));

            //     Collect all five balls
            collectPar->addSubActionPair(manip, std::make_shared<StartCollectAction>(*manip));

            // Then, in parallel...
            auto prepareFirePar = std::make_shared<ParallelAction>(robot.getMessageLogger());
            pushAction(prepareFirePar);
            
            //     Drive to the target
            prepareFirePar->addSubActionPair(db, std::make_shared<TankDriveFollowPathAction>(*db, "five_ball_auto_fire"));

            //     Stop collecting and prepare to fire
            prepareFirePar->addSubActionPair(manip, std::make_shared<StopCollectAction>(*manip));

            // Then, fire all five balls
            pushSubActionPair(manip, std::make_shared<FireAction>(*manip));
            
        }

        DroidFiveBallAutomode::~DroidFiveBallAutomode()
        {

        }
    }
}