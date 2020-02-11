#include "DroidEightBallAutomode.h"
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
        DroidEightBallAutomode::DroidEightBallAutomode(xero::base::Robot &robot) : 
            DroidAutoMode(robot, "NearSideEight", "Start on near side, score three, collect five, score five")
        {
            // Turret will track target as its default action
            // TODO: assign initial position to kinematic model?

            auto droid = getDroidSubsystem();
            auto db = droid->getTankDrive();
            auto manip = droid->getGamePieceManipulator();

            // Drive to the target
            pushSubActionPair(db, std::make_shared<TankDriveFollowPathAction>(*db, "eight_ball_auto_fire3"));

            // Fire all three balls
            pushSubActionPair(manip, std::make_shared<FireAction>(*manip));

            // Then, in parallel...
            auto parallel = std::make_shared<ParallelAction>(robot.getMessageLogger());
            pushAction(parallel);

            //     Drive to collect five balls
            parallel->addSubActionPair(db, std::make_shared<TankDriveFollowPathAction>(*db, "five_ball_auto_collect"));

            //     Collect balls until full and prepare to fire
            auto collectSeq = std::make_shared<SequenceAction>(robot.getMessageLogger());
            collectSeq->pushAction(std::make_shared<StartCollectAction>(*manip));
            collectSeq->pushAction(std::make_shared<StopCollectAction>(*manip));
            pushSubActionPair(manip, collectSeq);

            // Then, fire all five balls
            pushSubActionPair(manip, std::make_shared<FireAction>(*manip));
        }

        DroidEightBallAutomode::~DroidEightBallAutomode()
        {

        }
    }
}