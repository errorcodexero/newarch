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
#include <motorencodersubsystem/MotorEncoderGoToAction.h>

using namespace xero::base;
using namespace xero::misc;
namespace xero
{
    namespace droid
    {
        DroidEightBallAutomode::DroidEightBallAutomode(xero::base::Robot &robot, int variation) : 
            DroidAutoMode(robot, 
                (variation == 0) ? "NearSideEight" : "NearSideSix", 
                (variation == 0) ? "Start on near side, score three, collect five, score five" :
                                   "Start on near side, score three, collect three, score three")
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
            auto turretSeries = std::make_shared<SequenceAction>(robot.getMessageLogger());
            double target = (variation == 2 ? 20 : 0);
            turretSeries->pushSubActionPair(turret, std::make_shared<MotorEncoderGoToAction>(*turret, target, "goto", false), true);
            turretSeries->pushSubActionPair(turret, std::make_shared<FollowTargetAction>(*turret), false);
            parallel->addAction(turretSeries);

            // Get the balls ready
            parallel->addSubActionPair(conveyor, std::make_shared<ConveyorPrepareToEmitAction>(*conveyor));            

            // Drive to the shooting spot
            parallel->addSubActionPair(db, std::make_shared<TankDriveFollowPathAction>(*db, "eight_ball_auto_fire"));

            // Spin up the shooter
            parallel->addSubActionPair(shooter, std::make_shared<ShooterVelocityAction>(*shooter, 4150, Shooter::HoodPosition::Down), false) ;

            // Add the game piece manipulator stuff
            pushAction(parallel) ;           

            // Fire all three balls
            pushSubActionPair(manip, std::make_shared<FireAction>(*manip));         

            // Then, in parallel...
            parallel = std::make_shared<ParallelAction>(robot.getMessageLogger());
            pushAction(parallel);

            // Drive to collect five balls
            if (variation == 0)
            {
                parallel->addSubActionPair(db, std::make_shared<TankDriveFollowPathAction>(*db, "eight_ball_auto_collect")) ;
            }
            else
            {
                parallel->addSubActionPair(db, std::make_shared<TankDriveFollowPathAction>(*db, "eight_ball_auto_collect2")) ;                
            }

            // Collect all five balls
            auto collectSeries = std::make_shared<SequenceAction>(robot.getMessageLogger());
            collectSeries->pushSubActionPair(conveyor, std::make_shared<ConveyorPrepareToReceiveAction>(*conveyor)) ;
            collectSeries->pushSubActionPair(manip, std::make_shared<StartCollectAction>(*manip), false);
            parallel->addAction(collectSeries);

            // Stop collecting
            pushSubActionPair(manip, std::make_shared<StopCollectAction>(*manip));            

            // Then, in parallel...
            parallel = std::make_shared<ParallelAction>(robot.getMessageLogger());
            pushAction(parallel) ;

            // Prepare to emit
            parallel->addSubActionPair(conveyor, std::make_shared<ConveyorPrepareToEmitAction>(*conveyor)); 
            
            // Start looking for a target
            turretSeries = std::make_shared<SequenceAction>(robot.getMessageLogger());
            turretSeries->pushSubActionPair(turret, std::make_shared<MotorEncoderGoToAction>(*turret, 0, "goto", false), true);
            turretSeries->pushSubActionPair(turret, std::make_shared<FollowTargetAction>(*turret), false);
            parallel->addAction(turretSeries);  
            
            // Drive to the fire zone
            parallel->addSubActionPair(db, std::make_shared<TankDriveFollowPathAction>(*db, "eight_ball_auto_fire2", true));

            // Spin up shooter
            parallel->addSubActionPair(shooter, std::make_shared<ShooterVelocityAction>(*shooter, 4150, Shooter::HoodPosition::Down), false) ;

            // Then, fire all five balls
            pushSubActionPair(manip, std::make_shared<FireAction>(*manip));
        }

        DroidEightBallAutomode::~DroidEightBallAutomode()
        {
        }
    }
}