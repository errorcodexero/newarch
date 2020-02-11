#include "DroidThreeBallAutomode.h"
#include "droidsubsystem/DroidSubsystem.h"
#include "gamepiecemanipulator/FireAction.h"

#include <tankdrive/actions/TankDriveFollowPathAction.h>

using namespace xero::base;
using namespace xero::misc;
namespace xero
{
    namespace droid
    {
        DroidThreeBallAutomode::DroidThreeBallAutomode(xero::base::Robot &robot) : 
            DroidAutoMode(robot, "CenterThree", "Start in center and score the three balls")
        {
            // Turret will track target as its default action
            // TODO: assign initial position to kinematic model?

            auto droid = getDroidSubsystem();
            auto db = droid->getTankDrive();
            auto manip = droid->getGamePieceManipulator();

            // Drive to the target
            pushSubActionPair(db, std::make_shared<TankDriveFollowPathAction>(*db, "three_ball_auto"));

            // Fire all balls
            pushSubActionPair(manip, std::make_shared<FireAction>(*manip));
        }

        DroidThreeBallAutomode::~DroidThreeBallAutomode()
        {

        }
    }
}