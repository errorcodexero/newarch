#include "AutoModeXeroTest.h"
#include <Ranseur.h>
#include <bunnyarm/BunnyArm.h>
#include <bunnyarm/BunnyArmDeployAction.h>
#include <tubcollector/TubCollector.h>
#include <tubcollector/TubCollectorTubAction.h>
#include <tubcollector/TubCollectorEjectTubAction.h>
#include <ranseurcameratracker/DriveByVisionAction.h>
#include <tubmanipulatorsubsystem/TubManipulatorCollectAction.h>
#include <tubmanipulatorsubsystem/TubManipulatorDumpAction.h>
#include <tankdrive/actions/TankDriveFollowPathAction.h>
#include <singlemotorsubsystem/SingleMotorPowerAction.h>
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
        AutoModeXeroTest::AutoModeXeroTest(Robot &robot) : RanseurAutoModeBase(robot, "FetchTheTub", "Drive to center and pick up tub")
        {
            ActionPtr act ;
            auto &ranseur = dynamic_cast<Ranseur &>(getRobot()) ;
            auto bunnyarm = ranseur.getRanseurRobotSubsystem()->getBunnyArm() ;
            auto tubcollector = ranseur.getRanseurRobotSubsystem()->getTubManipulatorSubsystem()->getTubCollector() ;
            auto tubarm = ranseur.getRanseurRobotSubsystem()->getTubManipulatorSubsystem()->getTubArm() ;
            auto tubwrist = ranseur.getRanseurRobotSubsystem()->getTubManipulatorSubsystem()->getTubWrist() ;
            auto tankdrive = ranseur.getRanseurRobotSubsystem()->getTankDrive() ;
            auto camera = ranseur.getRanseurRobotSubsystem()->getCameraTracker() ;
            auto tubmanipulatorsubsytem = ranseur.getRanseurRobotSubsystem()->getTubManipulatorSubsystem() ;

            int which = robot.getSettingsParser().getInteger("automode:test:which") ;
            double power = robot.getSettingsParser().getDouble("automode:test:power") ;
            double duration = robot.getSettingsParser().getDouble("automode:test:duration") ;

            switch(which)
            {
                case 0:
                    // Run arm motor
                    act = std::make_shared<SingleMotorPowerAction>(*tubarm, power, duration) ;
                    pushSubActionPair(tubarm, act) ;
                    break ;

                case 1:
                    // Run Wrist motor
                    act = std::make_shared<SingleMotorPowerAction>(*tubwrist, power, duration) ;
                    pushSubActionPair(tubwrist, act) ;
                    break ;                

                case 2:
                    // TUB arm go to angle
                    act = std::make_shared<MotorEncoderGoToAction>(*tubarm, 0.0) ;
                    pushSubActionPair(tubarm, act) ;
                    break ;

                case 3:
                    // TUB wrist go to angle
                    act = std::make_shared<MotorEncoderGoToAction>(*tubwrist, 0.0) ;
                    pushSubActionPair(tubwrist, act) ;                
                    break ;

                case 4:
                    // Deploy the bunny arm
                    act = std::make_shared<BunnyArmDeployAction>(*bunnyarm, true) ;
                    pushSubActionPair(bunnyarm, act) ;
                    break ;

                case 5:
                    // Deploy the bunny arm then retract it again
                    act = std::make_shared<BunnyArmDeployAction>(*bunnyarm, true) ;
                    pushSubActionPair(bunnyarm, act) ;
                    pushAction(std::make_shared<DelayAction>(3.0)) ;
                    act = std::make_shared<BunnyArmDeployAction>(*bunnyarm, false) ;
                    pushSubActionPair(bunnyarm, act) ;                    
                    break ;   

                case 6:
                    // TUB arm go to angle
                    act = std::make_shared<MotorEncoderGoToAction>(*tubarm, 0.0) ;
                    pushSubActionPair(tubarm, act) ;
                    pushAction(std::make_shared<DelayAction>(3.0)) ;  
                    act = std::make_shared<MotorEncoderGoToAction>(*tubarm, 90.0) ;
                    pushSubActionPair(tubarm, act) ;                                      
                    break ;

                case 7:
                    // TUB arm go to angle
                    act = std::make_shared<MotorEncoderGoToAction>(*tubwrist, 0.0) ;
                    pushSubActionPair(tubwrist, act) ;
                    pushAction(std::make_shared<DelayAction>(3.0)) ;  
                    act = std::make_shared<MotorEncoderGoToAction>(*tubwrist, 90.0) ;
                    pushSubActionPair(tubwrist, act) ;                                      
                    break ;                       

           }
        }

        AutoModeXeroTest::~AutoModeXeroTest()
        {            
        }
    }
}
