#include "DroidEightBallAutomode.h"
#include "droidsubsystem/DroidSubsystem.h"
#include "gamepiecemanipulator/FireAction.h"
#include "gamepiecemanipulator/StartCollectAction.h"
#include "gamepiecemanipulator/StopCollectAction.h"
#include "turret/FollowTargetAction.h"
#include "gamepiecemanipulator/conveyor/ConveyorPrepareToEmitAction.h"
#include "gamepiecemanipulator/conveyor/ConveyorPrepareToReceiveAction.h"
#include "gamepiecemanipulator/conveyor/ConveyorSetBallCountAction.h"
#include "gamepiecemanipulator/shooter/ShooterVelocityAction.h"
#include <actions/ParallelAction.h>
#include <actions/DelayAction.h>
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
            auto droid = getDroidSubsystem();
            auto db = droid->getTankDrive();
            auto manip = droid->getGamePieceManipulator();
            auto conveyor = manip->getConveyor() ;
            auto turret = droid->getTurret() ;
            auto shooter = manip->getShooter() ;
            auto parallel = std::make_shared<ParallelAction>(robot.getMessageLogger());
            auto series = std::make_shared<SequenceAction>(robot.getMessageLogger()) ;

            // Set the initial ball count to 3
            pushSubActionPair(conveyor, std::make_shared<ConveyorSetBallAction>(*conveyor, 3), false) ;

            // Start looking for a target
            pushSubActionPair(turret, std::make_shared<FollowTargetAction>(*turret), false);

            // Get the balls ready
            parallel->addSubActionPair(conveyor, std::make_shared<ConveyorPrepareToEmitAction>(*conveyor));            

            // Drive to the target
            parallel->addSubActionPair(db, std::make_shared<TankDriveFollowPathAction>(*db, "eight_ball_auto_fire"));

            // Spin up the shooter
            parallel->addSubActionPair(shooter, std::make_shared<ShooterVelocityAction>(*shooter, 4150, true), false) ;

            // Add the game piece manipulator stuff
            pushAction(parallel) ;           

            // Fire all three balls
            pushSubActionPair(manip, std::make_shared<FireAction>(*manip));

            pushSubActionPair(conveyor, std::make_shared<ConveyorPrepareToReceiveAction>(*conveyor)) ;            

            // Then, in parallel...
            parallel = std::make_shared<ParallelAction>(robot.getMessageLogger());
            pushAction(parallel);
            
            // Drive to collect five balls
            parallel->addSubActionPair(db, std::make_shared<TankDriveFollowPathAction>(*db, "eight_ball_auto_collect"));

            // Collect all five balls
            parallel->addSubActionPair(manip, std::make_shared<StartCollectAction>(*manip), false);

            pushAction(std::make_shared<DelayAction>(robot.getMessageLogger(), 1.0)); 

            // Stop collecting
            pushSubActionPair(manip, std::make_shared<StopCollectAction>(*manip));            

            // Then, in parallel...
            parallel = std::make_shared<ParallelAction>(robot.getMessageLogger());
            pushAction(parallel) ;

            parallel->addSubActionPair(conveyor, std::make_shared<ConveyorPrepareToEmitAction>(*conveyor));   
            
            //     Drive to the target
            parallel->addSubActionPair(db, std::make_shared<TankDriveFollowPathAction>(*db, "eight_ball_auto_fire2", true));

            parallel->addSubActionPair(shooter, std::make_shared<ShooterVelocityAction>(*shooter, 4150, true), false) ;

            // Then, fire all five balls
            pushSubActionPair(manip, std::make_shared<FireAction>(*manip));
        }

        DroidEightBallAutomode::~DroidEightBallAutomode()
        {

        }
    }
}