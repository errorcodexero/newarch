#include "DroidThreeBallAutomode.h"
#include "droidsubsystem/DroidSubsystem.h"
#include "gamepiecemanipulator/FireAction.h"
#include "gamepiecemanipulator/conveyor/ConveyorSetBallCountAction.h"
#include "gamepiecemanipulator/conveyor/ConveyorPrepareToEmitAction.h"
#include "gamepiecemanipulator/shooter/ShooterVelocityAction.h"
#include "turret/FollowTargetAction.h"

#include <actions/ParallelAction.h>
#include <motorencodersubsystem/MotorEncoderGoToAction.h>
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

            auto droid = getDroidSubsystem();
            auto db = droid->getTankDrive();
            auto manip = droid->getGamePieceManipulator();
            auto turret = droid->getTurret() ;
            auto conveyor = manip->getConveyor() ;
            auto shooter = manip->getShooter();
            auto parallel = std::make_shared<ParallelAction>(robot.getMessageLogger());
            

            // Set the initial ball count to 3
            pushSubActionPair(conveyor, std::make_shared<ConveyorSetBallAction>(*conveyor, 3), false) ;
            
            // In parallel...
            pushAction(parallel);

            // Spin up the shooter
            parallel->addSubActionPair(shooter, std::make_shared<ShooterVelocityAction>(*shooter, 4150, Shooter::HoodPosition::Down), false) ;

            //     Prepare to emit
            parallel->addSubActionPair(conveyor, std::make_shared<ConveyorPrepareToEmitAction>(*conveyor));

            //     Start looking for a target
            auto turretSeries = std::make_shared<SequenceAction>(robot.getMessageLogger());
            double target = 0;
            turretSeries->pushSubActionPair(turret, std::make_shared<MotorEncoderGoToAction>(*turret, target, "goto", false), true);
            turretSeries->pushSubActionPair(turret, std::make_shared<FollowTargetAction>(*turret), false);
            parallel->addAction(turretSeries);

            // Fire all balls
            pushSubActionPair(manip, std::make_shared<FireAction>(*manip));

            // Drive forwards
            pushSubActionPair(db, std::make_shared<TankDriveFollowPathAction>(*db, "three_ball_auto_fire", true));
        }

        DroidThreeBallAutomode::~DroidThreeBallAutomode()
        {

        }
    }
}