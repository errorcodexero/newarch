#include "Bunny.h"
#include "automodes/BunnyAutoMode.h"
#include "bunnysubsystem/BunnySubsystem.h"
#include "bunnyoi/BunnyOISubsystem.h"
#include <shooter/ShooterEjectAutoBallAction.h>
#include <tankdrive/TankDrive.h>
#include <tankdrive/TankDriveDistanceAction.h>
#include <tankdrive/TankDriveCharAction.h>
#include <tankdrive/TankDriveAngleCharAction.h>
#include <tankdrive/TankDriveAngleAction.h>
#include <tankdrive/TankDriveTimedPowerAction.h>
#include <singlemotorsubsystem/SingleMotorPowerAction.h>
#include <DelayAction.h>
#include <ParallelAction.h>


using namespace xero::base ;

namespace xero {
    namespace bunny2018 {
        BunnyAutoMode::BunnyAutoMode(Robot &robot) : AutoController(robot) {
        }
        
        BunnyAutoMode::~BunnyAutoMode() {
        }

        void BunnyAutoMode::updateAutoMode(int sel, const std::string &gamedata) {
            AutoModePtr mode ;
            
            if (sel >= 0 && sel <= 9) {
                int min_crates = getRobot().getSettingsParser().getInteger("automode:1:min_crate_number") ;             
                mode = createGameAutoMode(sel + min_crates) ;
            }
            else {
                switch(sel) {
                    case 10:
                        mode = createTestMotors() ;
                        break ;

                    case 11:
                        mode = createDriveStraightChar() ;
                        break ;

                    case 12:
                        mode = createDriveStraightTest() ;
                        break ;

                    case 13:
                        mode = createShooterTestMode() ;
                        break ;             

                    default:
                        mode = nullptr ;
                        break ; 
                }
            }

            setAction(mode) ;
        }

        AutoModePtr BunnyAutoMode::createDriveStraightChar() {
            auto tankdrive = std::dynamic_pointer_cast<TankDrive>(getRobot().getDriveBase()) ;
            auto seq = std::make_shared<AutoMode>(getRobot().getMessageLogger(), "TankDriveStraightChar") ;
            auto act = std::make_shared<TankDriveCharAction>(*tankdrive, 2.0, 0.9) ;
            seq->pushSubActionPair(tankdrive, act) ;
            return seq ;
        }

        AutoModePtr BunnyAutoMode::createDriveStraightTest() {
            auto tankdrive = std::dynamic_pointer_cast<TankDrive>(getRobot().getDriveBase()) ;

            auto seq = std::make_shared<AutoMode>(getRobot().getMessageLogger(), "TankDriveStraight") ;
            auto act = std::make_shared<TankDriveDistanceAction>(*tankdrive, 276.0) ;           
            seq->pushSubActionPair(tankdrive, act) ;

            return seq ;
        }

        AutoModePtr BunnyAutoMode::createShooterTestMode() {
            ActionPtr act ;
            auto &robot = getRobot() ;
            Bunny &bunny = dynamic_cast<Bunny &>(robot) ;

            auto intake = bunny.getBunnySubsystem()->getIntake() ;
            auto tankdrive = bunny.getBunnySubsystem()->getTankDrive() ;
            auto collector = bunny.getBunnySubsystem()->getCollector() ;
            auto hopper = bunny.getBunnySubsystem()->getHopper() ;
            auto shooter = bunny.getBunnySubsystem()->getShooter() ;

            auto seq = std::make_shared<AutoMode>(getRobot().getMessageLogger(), "ShootTest") ;   

            act = std::make_shared<SingleMotorPowerAction>(*intake, -0.8) ;
            seq->pushSubActionPair(intake, act) ;   

            act = std::make_shared<ShooterStageBallAction>(*shooter) ;
            seq->pushSubActionPair(shooter, act) ;

            act = std::make_shared<DelayAction>(2.0) ;
            seq->pushAction(act) ;

            act = std::make_shared<ShooterEjectOneBallAction>(*shooter) ;
            seq->pushSubActionPair(shooter, act) ;

            act = std::make_shared<ShooterStageBallAction>(*shooter) ;
            seq->pushSubActionPair(shooter, act) ;

            act = std::make_shared<DelayAction>(2.0) ;
            seq->pushAction(act) ;

            act = std::make_shared<ShooterEjectOneBallAction>(*shooter) ;
            seq->pushSubActionPair(shooter, act) ;

            act = std::make_shared<ShooterStageBallAction>(*shooter) ;
            seq->pushSubActionPair(shooter, act) ;

            act = std::make_shared<DelayAction>(2.0) ;
            seq->pushAction(act) ;

            act = std::make_shared<ShooterEjectOneBallAction>(*shooter) ;
            seq->pushSubActionPair(shooter, act) ;

            act = std::make_shared<ShooterStageBallAction>(*shooter) ;
            seq->pushSubActionPair(shooter, act) ;

            act = std::make_shared<DelayAction>(2.0) ;
            seq->pushAction(act) ;

            act = std::make_shared<ShooterEjectOneBallAction>(*shooter) ;
            seq->pushSubActionPair(shooter, act) ;

            act = std::make_shared<ShooterStageBallAction>(*shooter) ;
            seq->pushSubActionPair(shooter, act) ;

            act = std::make_shared<DelayAction>(2.0) ;
            seq->pushAction(act) ;

            act = std::make_shared<ShooterEjectOneBallAction>(*shooter) ;
            seq->pushSubActionPair(shooter, act) ;

            act = std::make_shared<ShooterStageBallAction>(*shooter) ;
            seq->pushSubActionPair(shooter, act) ;

            act = std::make_shared<DelayAction>(2.0) ;
            seq->pushAction(act) ;

            act = std::make_shared<ShooterEjectOneBallAction>(*shooter) ;
            seq->pushSubActionPair(shooter, act) ;

            return seq ;
        }

        AutoModePtr BunnyAutoMode::createTestMotors() {
            ActionPtr act ;
            auto &robot = getRobot() ;
            Bunny &bunny = dynamic_cast<Bunny &>(robot) ;

            auto intake = bunny.getBunnySubsystem()->getIntake() ;
            auto tankdrive = bunny.getBunnySubsystem()->getTankDrive() ;
            auto collector = bunny.getBunnySubsystem()->getCollector() ;
            auto hopper = bunny.getBunnySubsystem()->getHopper() ;
            auto shooter = bunny.getBunnySubsystem()->getShooter() ;

            auto seq = std::make_shared<AutoMode>(getRobot().getMessageLogger(), "TestMotors") ;
            assert(shooter != nullptr) ;

            act = std::make_shared<SingleMotorPowerAction>(*shooter, 0.5, 2.0) ;
            seq->pushSubActionPair(shooter, act) ;      

            act = std::make_shared<SingleMotorPowerAction>(*shooter, -0.5, 2.0) ;
            seq->pushSubActionPair(shooter, act) ;              

            act = std::make_shared<SingleMotorPowerAction>(*collector, 0.5, 2.0) ;
            seq->pushSubActionPair(collector, act) ;        

            act = std::make_shared<SingleMotorPowerAction>(*collector, -0.5, 2.0) ;
            seq->pushSubActionPair(collector, act) ;    

            act = std::make_shared<SingleMotorPowerAction>(*hopper, 0.5, 2.0) ;
            seq->pushSubActionPair(hopper, act) ;       

            act = std::make_shared<SingleMotorPowerAction>(*hopper, -0.5, 2.0) ;
            seq->pushSubActionPair(hopper, act) ;   

            act = std::make_shared<SingleMotorPowerAction>(*intake, 0.5, 2.0) ;
            seq->pushSubActionPair(intake, act) ;       

            act = std::make_shared<SingleMotorPowerAction>(*intake, -0.5, 2.0) ;
            seq->pushSubActionPair(intake, act) ;
            

            return seq ;          
        }        

        AutoModePtr BunnyAutoMode::createGameAutoMode(int number_of_crates) {
            ActionPtr act ;

            std::string name("crates ") ;
            name += std::to_string(number_of_crates) ;
            auto seq = std::make_shared<AutoMode>(getRobot().getMessageLogger(), name) ;

            auto &robot = getRobot() ;
            Bunny &bunny = dynamic_cast<Bunny &>(robot) ;

            auto tankdrive = bunny.getBunnySubsystem()->getTankDrive() ;
            auto collector = bunny.getBunnySubsystem()->getCollector() ;
            auto hopper = bunny.getBunnySubsystem()->getHopper() ;
            auto shooter = bunny.getBunnySubsystem()->getShooter() ;
            auto intake = bunny.getBunnySubsystem()->getIntake() ;

            // Stage a ball
            act = std::make_shared<ShooterStageBallAction>(*shooter) ;
            seq->pushSubActionPair(shooter, act) ;

            // Eject existing ball
            act = std::make_shared<ShooterEjectOneBallAction>(*shooter) ;
            seq->pushSubActionPair(shooter, act) ;

            // Collector to state on
            act = std::make_shared<SingleMotorPowerAction>(*collector, "collector:power:fwd") ;
            seq->pushSubActionPair(collector, act) ;

            // Hopper to state on
            act = std::make_shared<SingleMotorPowerAction>(*hopper, "hopper:power:fwd") ;
            seq->pushSubActionPair(hopper, act) ;

            // Intake to state on
            act = std::make_shared<SingleMotorPowerAction>(*intake, "intake:power:fwd") ;
            seq->pushSubActionPair(intake, act) ;   

            // Shooter to state auto shoot action
            auto shootact = std::make_shared<ShooterEjectAutoBallAction>(*shooter) ;
            addCrateLocations(shootact, number_of_crates) ;
            seq->pushSubActionPair(shooter, shootact, false) ;
            
            // Drive straight, collecting and depositing as we go
            act = std::make_shared<TankDriveDistanceAction>(*tankdrive, "automode:1:forward_distance") ;
            seq->pushSubActionPair(tankdrive, act) ;

            return seq ;
        }

        void BunnyAutoMode::addCrateLocations(std::shared_ptr<ShooterEjectAutoBallAction> act, int numcrates) {
            
            double crate_width = getRobot().getSettingsParser().getDouble("automode:1:crate_width") ;
            double pos = crate_width * 2.5 ;

            std::cout << "Crates" ;
            for(int i = 0 ; i < numcrates ; i++) {
                std::cout << " " << pos ;
                act->addCrateLocation(pos) ;
                pos += crate_width * 2 ;
            }
            std::cout << std::endl ;
        }
    }
}
