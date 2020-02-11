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
            auto parallel = std::make_shared<ParallelAction>(robot.getMessageLogger());
            pushAction(parallel);

            //     Drive to the balls
            parallel->addSubActionPair(db, std::make_shared<TankDriveFollowPathAction>(*db, "five_ball_auto_collect"));

            //     Collect balls until full and prepare to fire
            auto collectSeq = std::make_shared<SequenceAction>(robot.getMessageLogger());
            collectSeq->pushAction(std::make_shared<StartCollectAction>(*manip));
            collectSeq->pushAction(std::make_shared<StopCollectAction>(*manip));
            pushSubActionPair(manip, collectSeq);

            // Then, fire all balls
            pushSubActionPair(manip, std::make_shared<FireAction>(*manip));
            
        }

        DroidFiveBallAutomode::~DroidFiveBallAutomode()
        {

        }
    }
}