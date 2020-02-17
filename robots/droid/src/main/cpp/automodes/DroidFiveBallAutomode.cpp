#include "DroidFiveBallAutomode.h"
#include "droidsubsystem/DroidSubsystem.h"
#include "gamepiecemanipulator/FireAction.h"
#include "gamepiecemanipulator/StartCollectAction.h"
#include "gamepiecemanipulator/StopCollectAction.h"
#include "gamepiecemanipulator/conveyor/ConveyorSetBallCountAction.h"
#include "gamepiecemanipulator/conveyor/ConveyorPrepareToEmitAction.h"
#include "gamepiecemanipulator/conveyor/ConveyorPrepareToReceiveAction.h"
#include "gamepiecemanipulator/shooter/ShooterVelocityAction.h"
#include "motorencodersubsystem/MotorEncoderGotoAction.h"
#include "turret/FollowTargetAction.h"
#include <actions/ParallelAction.h>
#include <actions/DelayAction.h>
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
            auto droid = getDroidSubsystem();
            auto db = droid->getTankDrive();
            auto manip = droid->getGamePieceManipulator();
            auto conveyor = manip->getConveyor() ;
            auto turret = droid->getTurret() ;
            auto shooter = manip->getShooter() ;
            auto parallel = std::make_shared<ParallelAction>(robot.getMessageLogger());

             // Set the initial ball count to 3
            pushSubActionPair(conveyor, std::make_shared<ConveyorSetBallAction>(*conveyor, 3), false) ;

            // Get the balls ready
            pushSubActionPair(conveyor, std::make_shared<ConveyorPrepareToReceiveAction>(*conveyor));        

            // Collect all five balls
            parallel->addSubActionPair(manip, std::make_shared<StartCollectAction>(*manip), false);

            // Drive to the get other two balls
            parallel->addSubActionPair(db, std::make_shared<TankDriveFollowPathAction>(*db, "five_ball_auto_collect"));

            // Move the motgor
            parallel->addSubActionPair(turret, std::make_shared<MotorEncoderGoToAction>(*turret, 0.0, "goto", false), true);

            pushAction(parallel) ;

            // Stop collecting
            pushSubActionPair(manip, std::make_shared<StopCollectAction>(*manip)); 
            pushSubActionPair(turret, std::make_shared<FollowTargetAction>(*turret), false);

            parallel = std::make_shared<ParallelAction>(robot.getMessageLogger());
            pushAction(parallel) ;

            parallel->addSubActionPair(conveyor, std::make_shared<ConveyorPrepareToEmitAction>(*conveyor));   
            
            //     Drive to the target
            parallel->addSubActionPair(db, std::make_shared<TankDriveFollowPathAction>(*db, "five_ball_auto_fire", true));

            parallel->addSubActionPair(shooter, std::make_shared<ShooterVelocityAction>(*shooter, 4150, true), false) ;

            // Then, fire all five balls
            pushSubActionPair(manip, std::make_shared<FireAction>(*manip));
            
        }

        DroidFiveBallAutomode::~DroidFiveBallAutomode()
        {

        }
    }
}