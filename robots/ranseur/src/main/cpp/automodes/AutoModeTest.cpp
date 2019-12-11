#include "AutoModeTest.h"
#include <Ranseur.h>
#include <bunnyarm/BunnyArm.h>
#include <bunnyarm/BunnyArmDeployAction.h>
#include <tubcollector/TubCollector.h>
#include <tubcollector/TubCollectorTubAction.h>
#include <tubcollector/TubCollectorEjectTubAction.h>
#include <tubcollector/TubCollectorDutyCycleAction.h>
#include <tubcollector/TubCollectorTubAction.h>
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
#include <tubtoucher/TubToucherDeployAction.h>

using namespace xero::base ;

namespace xero
{
    namespace ranseur 
    {
        AutoModeTest::AutoModeTest(Robot &robot) : RanseurAutoModeBase(robot, "TestingMode", "The Automode to Test Stuff")
        {
            ActionPtr act ;
            std::shared_ptr<ParallelAction> par ;
            std::shared_ptr<SequenceAction> seq ;
            auto &ranseur = dynamic_cast<Ranseur &>(getRobot()) ;
            auto bunnyarm = ranseur.getRanseurRobotSubsystem()->getBunnyArm() ;
            auto tubtoucher = ranseur.getRanseurRobotSubsystem()->getTubToucher() ;
            auto tubcollector = ranseur.getRanseurRobotSubsystem()->getTubManipulatorSubsystem()->getTubCollector() ;
            auto tubarm = ranseur.getRanseurRobotSubsystem()->getTubManipulatorSubsystem()->getTubArm() ;
            auto tubwrist = ranseur.getRanseurRobotSubsystem()->getTubManipulatorSubsystem()->getTubWrist() ;
            auto tankdrive = ranseur.getRanseurRobotSubsystem()->getTankDrive() ;
            auto camera = ranseur.getRanseurRobotSubsystem()->getCameraTracker() ;
            auto tubmanipulatorsubsytem = ranseur.getRanseurRobotSubsystem()->getTubManipulatorSubsystem() ;

            int which = getRobot().getSettingsParser().getInteger("automode:test:which")  ;
            double power = getRobot().getSettingsParser().getDouble("automode:test:power") ;
            double duration = getRobot().getSettingsParser().getDouble("automode:test:duration") ;
            double angle = getRobot().getSettingsParser().getDouble("automode:test:angle") ;
            
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
                    act = std::make_shared<MotorEncoderGoToAction>(*tubarm, angle) ;
                    pushSubActionPair(tubarm, act) ;
                break ;

                case 3 :
                    act = std::make_shared<MotorEncoderGoToAction>(*tubwrist, angle) ;
                    pushSubActionPair(tubwrist, act) ;
                break ;

                case 4 :
                    act = std::make_shared<BunnyArmDeployAction>(*bunnyarm, true) ;
                    pushSubActionPair(bunnyarm, act) ;
                    pushAction(std::make_shared<DelayAction>(3.0)) ;  
                    act = std::make_shared<BunnyArmDeployAction>(*bunnyarm, false) ;
                    pushSubActionPair(bunnyarm, act) ;                                      
                break ;

                case 5 :

                break ;
                
                case 6 :
                    act = std::make_shared<TubToucherDeployAction>(*tubtoucher, true) ;
                    pushSubActionPair(tubtoucher, act) ;
                    pushAction(std::make_shared<DelayAction>(3.0)) ;
                    act = std::make_shared<TubToucherDeployAction>(*tubtoucher, false) ;
                    pushSubActionPair(tubtoucher, act) ;
                break ;

                case 7:
                    act = std::make_shared<TubCollectorDutyCycleAction>(*tubcollector, power) ;
                    pushSubActionPair(tubcollector, act) ;                
                    pushAction(std::make_shared<DelayAction>(30.0)) ; 
                    act = std::make_shared<TubCollectorDutyCycleAction>(*tubcollector, 0.0) ;  
                    pushSubActionPair(tubcollector, act) ;                                                           
                    break ;

                case 8:
                    par = std::make_shared<ParallelAction>() ;

                    act = std::make_shared<MotorEncoderGoToAction>(*tubarm, -38) ;
                    par->addSubActionPair(tubarm, act, true) ;

                    act = std::make_shared<MotorEncoderGoToAction>(*tubwrist, 10) ;
                    par->addSubActionPair(tubwrist, act, true) ;

                    act = std::make_shared<TubCollectorTubAction>(*tubcollector) ;
                    par->addSubActionPair(tubcollector, act, true) ; 

                    pushAction(par) ;

                    par = std::make_shared<ParallelAction>() ;

                    act = std::make_shared<MotorEncoderGoToAction>(*tubarm, 90) ;
                    par->addSubActionPair(tubarm, act, true) ;

                    seq = std::make_shared<SequenceAction>(getRobot().getMessageLogger()) ;

                    act = std::make_shared<MotorEncoderGoToAction>(*tubwrist, 45) ;
                    seq->pushSubActionPair(tubwrist, act, true) ;

                    act = std::make_shared<MotorEncoderGoToAction>(*tubwrist, -160) ;
                    seq->pushSubActionPair(tubwrist, act, true) ;                

                    par->addAction(seq) ;
                    pushAction(par) ;
                    break ;
                    
                case 9:
                    par = std::make_shared<ParallelAction>() ;

                    act = std::make_shared<MotorEncoderGoToAction>(*tubarm, -38) ;
                    par->addSubActionPair(tubarm, act, true) ;

                    act = std::make_shared<MotorEncoderGoToAction>(*tubwrist, 10) ;
                    par->addSubActionPair(tubwrist, act, true) ;

                    pushAction(par) ;

                    act = std::make_shared<DelayAction>(3.0) ;
                    pushAction(act) ;

                    par = std::make_shared<ParallelAction>() ;

                    act = std::make_shared<MotorEncoderGoToAction>(*tubarm, 90) ;
                    par->addSubActionPair(tubarm, act, true) ;

                    seq = std::make_shared<SequenceAction>(getRobot().getMessageLogger()) ;

                    act = std::make_shared<MotorEncoderGoToAction>(*tubwrist, 45) ;
                    seq->pushSubActionPair(tubwrist, act, true) ;

                    act = std::make_shared<MotorEncoderGoToAction>(*tubwrist, -166) ;
                    seq->pushSubActionPair(tubwrist, act, true) ;                

                    par->addAction(seq) ;
                    pushAction(par) ;
                    break ;
            }

        }

        AutoModeTest::~AutoModeTest()
        {            
        }
    }
}
