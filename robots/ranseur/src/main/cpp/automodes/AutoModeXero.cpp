#include "AutoModeXero.h"
#include <Ranseur.h>
#include <bunnyarm/BunnyArm.h>
#include <bunnyarm/BunnyArmDeployAction.h>
#include <tubtoucher/TubToucher.h>
#include <tubtoucher/TubToucherDeployAction.h>
#include <tubcollector/TubCollector.h>
#include <tubcollector/TubCollectorTubAction.h>
#include <tubcollector/TubCollectorEjectTubAction.h>
#include <ranseurcameratracker/DriveByVisionAction.h>
#include <tubmanipulatorsubsystem/TubManipulatorCollectAction.h>
#include <tubmanipulatorsubsystem/TubManipulatorDumpAction.h>
#include <tankdrive/actions/TankDriveFollowPathAction.h>
#include <motorencodersubsystem/MotorEncoderGoToAction.h>
#include <actions/DelayAction.h>
#include <actions/SequenceAction.h>
#include <actions/TerminateAction.h>
#include <memory>

using namespace xero::base ;

namespace xero
{
    namespace ranseur 
    {
        AutoModeXero::AutoModeXero(Robot &robot) : RanseurAutoModeBase(robot, "Mode 0", "The one and only automode")
        {
            ActionPtr act ;
            auto &ranseur = dynamic_cast<Ranseur &>(getRobot()) ;
            auto bunnyarm = ranseur.getRanseurRobotSubsystem()->getBunnyArm() ;
            auto tubtoucher = ranseur.getRanseurRobotSubsystem()->getTubToucher() ;
            auto tubcollector = ranseur.getRanseurRobotSubsystem()->getTubManipulatorSubsystem()->getTubCollector() ;
            auto tubarm = ranseur.getRanseurRobotSubsystem()->getTubManipulatorSubsystem()->getTubArm() ;
            auto tubwrist = ranseur.getRanseurRobotSubsystem()->getTubManipulatorSubsystem()->getTubWrist() ;
            auto tankdrive = ranseur.getRanseurRobotSubsystem()->getTankDrive() ;
            auto camera = ranseur.getRanseurRobotSubsystem()->getCameraTracker() ;
            auto tubmanipulatorsubsytem = ranseur.getRanseurRobotSubsystem()->getTubManipulatorSubsystem() ;
            auto parallel = std::make_shared<ParallelAction>() ;

            double armangle = robot.getSettingsParser().getDouble("tubarm:collect:pos") ;
            double wristangle = robot.getSettingsParser().getDouble("tubwrist:collect:pos") ;
           
            // add parallel to sequence
            // everything else contained in parallel -> 2 seq actions
            pushAction(parallel) ;

            //// 1ST SEQUENCE ////
            auto sequence = std::make_shared<SequenceAction>(robot.getMessageLogger()) ;
            parallel->addAction(sequence) ;                   
            auto path = std::make_shared<TankDriveFollowPathAction>(*tankdrive, "BunnyAutoMode_PathToTub") ;

            //
            // Delay for 2 seconds before we start looking for the terminate condition, this gives us enough
            // time to let the arm get out of the way of the camera
            //
            auto term = std::make_shared<TerminateAction>(tankdrive, path , ranseur, 2.0) ;
            term->addTerminator(camera) ; 
            sequence->pushAction(term) ;
            sequence->pushSubActionPair(tubtoucher, std::make_shared<TubToucherDeployAction>(*tubtoucher, true), false) ;            
            sequence->pushAction(std::make_shared<DriveByVisionAction>(*tankdrive, *camera)) ;

            //// 2ND SEQUENCE ////
            sequence = std::make_shared<SequenceAction>(robot.getMessageLogger()) ;
            parallel->addAction(sequence) ;
            sequence->pushAction(std::make_shared<BunnyArmDeployAction>(*bunnyarm, true)) ;
            sequence->pushAction(std::make_shared<DelayAction>(0.5)) ;
            sequence->pushSubActionPair(bunnyarm, std::make_shared<BunnyArmDeployAction>(*bunnyarm, false)) ;
            sequence->pushSubActionPair(tubarm, std::make_shared<MotorEncoderGoToAction>(*tubarm, armangle)) ;
            sequence->pushSubActionPair(tubwrist, std::make_shared<MotorEncoderGoToAction>(*tubwrist, wristangle)) ;
            sequence->pushAction(std::make_shared<TubManipulatorCollectAction>(*tubmanipulatorsubsytem)) ;
            sequence->pushAction(std::make_shared<TubManipulatorDumpAction>(*tubmanipulatorsubsytem)) ;
        }

        AutoModeXero::~AutoModeXero()
        {            
        }
    }
}
