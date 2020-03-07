#include "DroidTenAutomode.h"
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
        DroidTenAutomode::DroidTenAutomode(xero::base::Robot &robot) : 
            DroidAutoMode(robot, "TenBall", "Near side ten ball")
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
            series->pushSubActionPair(conveyor, std::make_shared<ConveyorSetBallAction>(*conveyor, 3), false) ;

            // Get the balls ready
            series->pushSubActionPair(conveyor, std::make_shared<ConveyorPrepareToReceiveAction>(*conveyor));        

            // Collect all five balls
            series->pushSubActionPair(manip, std::make_shared<StartCollectAction>(*manip), false);

            // Add the series ball collect actions
            parallel->addAction(series) ;

            // Drive to the get other two balls
            parallel->addSubActionPair(db, std::make_shared<TankDriveFollowPathAction>(*db, "ten_ball_auto_collect1"));

            // Move the turret to the front position
            parallel->addSubActionPair(turret, std::make_shared<MotorEncoderGoToAction>(*turret, 0.0, "goto", false), true);

            // Crank up the shooter wheel so its ready
            parallel->addSubActionPair(shooter, std::make_shared<ShooterVelocityAction>(*shooter, 4150, Shooter::HoodPosition::Down), false) ;            

            // Do this parallel (and series) set of actions
            pushAction(parallel) ;

            // Move to a spot and fire
            parallel = std::make_shared<ParallelAction>(robot.getMessageLogger());
            pushAction(parallel) ;

            // Now get ready to fire
            series = std::make_shared<SequenceAction>(robot.getMessageLogger());
            series->pushSubActionPair(turret, std::make_shared<FollowTargetAction>(*turret), false);
            series->pushSubActionPair(manip, std::make_shared<StopCollectAction>(*manip)); 
            series->pushSubActionPair(conveyor, std::make_shared<ConveyorPrepareToEmitAction>(*conveyor)); 
            parallel->addAction(series);

            // Drive to the target
            parallel->addSubActionPair(db, std::make_shared<TankDriveFollowPathAction>(*db, "ten_ball_auto_fire1", true));

            // Then, fire all five balls
            pushSubActionPair(manip, std::make_shared<FireAction>(*manip));

            parallel = std::make_shared<ParallelAction>(robot.getMessageLogger());
            pushAction(parallel) ;
            series = std::make_shared<SequenceAction>(robot.getMessageLogger()) ;

            // Get the balls ready
            series->pushSubActionPair(conveyor, std::make_shared<ConveyorPrepareToReceiveAction>(*conveyor));        

            // Collect all five balls
            series->pushSubActionPair(manip, std::make_shared<StartCollectAction>(*manip), false);

            // Add the series ball collect actions
            parallel->addAction(series) ;

            // Drive to the get other two balls
            parallel->addSubActionPair(db, std::make_shared<TankDriveFollowPathAction>(*db, "ten_ball_auto_collect2"));

            // Now get ready to fire
            series = std::make_shared<SequenceAction>(robot.getMessageLogger());
            series->pushSubActionPair(manip, std::make_shared<StopCollectAction>(*manip)); 
            series->pushSubActionPair(conveyor, std::make_shared<ConveyorPrepareToEmitAction>(*conveyor)); 
            parallel->addAction(series);

            parallel = std::make_shared<ParallelAction>(robot.getMessageLogger());
            pushAction(parallel) ;            

            // Drive to the target
            parallel->addSubActionPair(db, std::make_shared<TankDriveFollowPathAction>(*db, "ten_ball_auto_fire2", true));

            // Then, fire all five balls
            pushSubActionPair(manip, std::make_shared<FireAction>(*manip));
        }

        DroidTenAutomode::~DroidTenAutomode()
        {
        }
    }
}