#include "AutoModeTest.h"
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
#include <actions/DelayAction.h>
#include <actions/SequenceAction.h>
#include <actions/TerminateAction.h>
#include <memory>
#include <singlemotorsubsystem/SingleMotorPowerAction.h>
#include <motorencodersubsystem/MotorEncoderGoToAction.h>

using namespace xero::base ;

namespace xero
{
    namespace ranseur 
    {
        AutoModeTest::AutoModeTest(Robot &robot) : RanseurAutoModeBase(robot, "TestingMode", "The Automode to Test Stuff")
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

            int which = getRobot().getSettingsParser().getInteger("which")  ;
            double power = getRobot().getSettingsParser().getDouble("power") ;
            double duration = getRobot().getSettingsParser().getDouble("duration") ;
            
            switch (which) {

                case 0 :
                    act = std::make_shared<SingleMotorPowerAction>(*tubarm, power, duration) ;
                    pushSubActionPair(tubarm, act) ;
                break ;

                case 1 :
                    act = std::make_shared<SingleMotorPowerAction>(*tubwrist, power, duration) ;
                    pushSubActionPair(tubwrist, act) ;
                break ;

                case 2 :
                    act = std::make_shared<MotorEncoderGoToAction>(*tubarm, 0) ;
                    pushSubActionPair(tubarm, act) ;
                break ;

                case 3 :
                    act = std::make_shared<MotorEncoderGoToAction>(*tubwrist, 90) ;
                    pushSubActionPair(tubwrist, act) ;
                break ;

                case 4 :
                    act = std::make_shared<BunnyArmDeployAction>(*bunnyarm, true) ;
                    pushSubActionPair(bunnyarm, act) ;
                break ;

                case 5 :
                    act = std::make_shared<BunnyArmDeployAction>(*bunnyarm, false) ;
                    pushSubActionPair(bunnyarm, act) ;
                break ;

            }

        }

        AutoModeTest::~AutoModeTest()
        {            
        }
    }
}
