#include "Bunny.h"
#include "automodes/BunnyAutoMode.h"
#include "bunnysubsystem/BunnySubsystem.h"
#include "bunnyoi/BunnyOISubsystem.h"
#include <shooter/ShooterEjectOneBallAction.h>
#include <shooter/ShooterStageBallAction.h>
#include <tankdrive/TankDrive.h>
#include <tankdrive/TankDriveDistanceAction.h>
#include <tankdrive/TankDriveCharAction.h>
#include <tankdrive/TankDriveAngleCharAction.h>
#include <tankdrive/TankDriveAngleAction.h>
#include <tankdrive/TankDriveTimedPowerAction.h>
#include <singlemotorsubsystem/SingleMotorPowerAction.h>
#include <sorter/SorterPowerAction.h>
#include <sorter/SorterSortAction.h>
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
            ActionSequencePtr mode ;
			
			int min_crates = getRobot().getSettingsParser().getInteger("automode:1:min_crate_number") ;

			switch(sel) {
				case 0:
					
				case 1:
					
				case 2:
					
				case 3:
					
				case 4:
					
				case 5:
					
				case 6:
					
				case 7:
					
				case 8:
					
				case 9:
					mode = createGameAutoMode(sel + min_crates) ;
					break ;

				case 10:
					mode = createTestAuto() ;
					break ;

				case 11:
					mode = createRotateChar() ;
					break ;

				case 12:
					mode = createDriveStraightTest() ;
					break ;

				case 13:
					mode = createRotatePos90Test() ;
					break ;

				case 14:
					mode = createRotateNeg90Test() ;
					break ;

				case 15:
					mode = createDriveSquareTest() ;
					break ;

				case 16:
					mode = createDriveStraightChar() ;				
					break ;

				case 17:
					mode = createStraightBackAutomode() ;
					break ;		

				case 18:
					mode = createRotateSorterAutoMode() ;
					break ;				

				default:
					mode = nullptr ;
					break ; 
			}

            setAction(mode) ;
        }

		ActionSequencePtr BunnyAutoMode::createDriveStraightChar() {
            auto tankdrive = std::dynamic_pointer_cast<TankDrive>(getRobot().getDriveBase()) ;
            auto seq = std::make_shared<ActionSequence>(getRobot().getMessageLogger(), "TankDriveStraightChar") ;
            auto act = std::make_shared<TankDriveCharAction>(*tankdrive, 10.0, 1.0) ;
			seq->pushSubActionPair(tankdrive, act) ;
			return seq ;

		}

		ActionSequencePtr BunnyAutoMode::createRotateChar() {
            auto tankdrive = std::dynamic_pointer_cast<TankDrive>(getRobot().getDriveBase()) ;

            auto seq = std::make_shared<ActionSequence>(getRobot().getMessageLogger(), "TankDriveRotateChar") ;
            auto act = std::make_shared<TankDriveAngleCharAction>(*tankdrive, 5.0, 0.0, 1.0, 0.1) ;			
			seq->pushSubActionPair(tankdrive, act) ;

			return seq ;
		}

		ActionSequencePtr BunnyAutoMode::createDriveStraightTest() {
            auto tankdrive = std::dynamic_pointer_cast<TankDrive>(getRobot().getDriveBase()) ;

            auto seq = std::make_shared<ActionSequence>(getRobot().getMessageLogger(), "TankDriveStraight") ;
            auto act = std::make_shared<TankDriveDistanceAction>(*tankdrive, 48.0) ;			
			seq->pushSubActionPair(tankdrive, act) ;

			return seq ;
		}

		ActionSequencePtr BunnyAutoMode::createRotatePos90Test() {
            auto tankdrive = std::dynamic_pointer_cast<TankDrive>(getRobot().getDriveBase()) ;

            auto seq = std::make_shared<ActionSequence>(getRobot().getMessageLogger(), "TankDriveRotate") ;
            auto act = std::make_shared<TankDriveAngleAction>(*tankdrive, 90.0) ;
			seq->pushSubActionPair(tankdrive, act) ;

			return seq ;
		}

		ActionSequencePtr BunnyAutoMode::createRotateNeg90Test() {
            auto tankdrive = std::dynamic_pointer_cast<TankDrive>(getRobot().getDriveBase()) ;

            auto seq = std::make_shared<ActionSequence>(getRobot().getMessageLogger(), "TankDriveRotate") ;
            auto act = std::make_shared<TankDriveAngleAction>(*tankdrive, -90.0) ;
			seq->pushSubActionPair(tankdrive, act) ;

			return seq ;
		}		

		ActionSequencePtr BunnyAutoMode::createDriveSquareTest() {
			ActionPtr act ;

            auto tankdrive = std::dynamic_pointer_cast<TankDrive>(getRobot().getDriveBase()) ;

            auto seq = std::make_shared<ActionSequence>(getRobot().getMessageLogger(), "TankDriveSquare") ;

            act = std::make_shared<TankDriveDistanceAction>(*tankdrive, 60.0) ;
			seq->pushSubActionPair(tankdrive, act) ;

            act = std::make_shared<TankDriveDistanceAction>(*tankdrive, -60.0) ;
			seq->pushSubActionPair(tankdrive, act) ;

            act = std::make_shared<TankDriveDistanceAction>(*tankdrive, 60.0) ;
			seq->pushSubActionPair(tankdrive, act) ;						

            act = std::make_shared<TankDriveAngleAction>(*tankdrive, 90.0) ;			
			seq->pushSubActionPair(tankdrive, act) ;						

            act = std::make_shared<TankDriveDistanceAction>(*tankdrive, 60.0) ;
			seq->pushSubActionPair(tankdrive, act) ;

            act = std::make_shared<TankDriveAngleAction>(*tankdrive, 90.0) ;			
			seq->pushSubActionPair(tankdrive, act) ;

            act = std::make_shared<TankDriveDistanceAction>(*tankdrive, 60.0) ;
			seq->pushSubActionPair(tankdrive, act) ;

            act = std::make_shared<TankDriveAngleAction>(*tankdrive, 90.0) ;			
			seq->pushSubActionPair(tankdrive, act) ;

            act = std::make_shared<TankDriveDistanceAction>(*tankdrive, 60.0) ;
			seq->pushSubActionPair(tankdrive, act) ;

            act = std::make_shared<TankDriveAngleAction>(*tankdrive, 90.0) ;			
			seq->pushSubActionPair(tankdrive, act) ;									

			return seq ;
		}	

		ActionSequencePtr BunnyAutoMode::createStraightBackAutomode() {
			return nullptr ;
		}


		ActionSequencePtr BunnyAutoMode::createRotateSorterAutoMode() {
			xero::base::ActionPtr act ;
			auto &robot = getRobot() ;
			Bunny &bunny = dynamic_cast<Bunny &>(robot) ;
			auto sorter = bunny.getBunnySubsystem()->getSorter() ;
            auto seq = std::make_shared<ActionSequence>(getRobot().getMessageLogger(), "SorterPower") ;
            act = std::make_shared<SorterPowerAction>(*sorter, 0.25) ;
			seq->pushSubActionPair(sorter, act) ;			

			return seq ;
		}

		ActionSequencePtr BunnyAutoMode::createTestAuto() {
			ActionPtr act ;
			auto &robot = getRobot() ;
			Bunny &bunny = dynamic_cast<Bunny &>(robot) ;

			auto intake = bunny.getBunnySubsystem()->getIntake() ;
			auto sorter = bunny.getBunnySubsystem()->getSorter() ;
			auto tankdrive = bunny.getBunnySubsystem()->getTankDrive() ;
			auto collector = bunny.getBunnySubsystem()->getCollector() ;
			auto hopper = bunny.getBunnySubsystem()->getHopper() ;
			auto shooter = bunny.getBunnySubsystem()->getShooter() ;

            auto seq = std::make_shared<ActionSequence>(getRobot().getMessageLogger(), "TestMotors") ;

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
			seq->pushSubActionPair(intake, act)	;

			act = std::make_shared<SorterPowerAction>(*sorter, 0.5) ;
			seq->pushSubActionPair(sorter, act) ;

			act = std::make_shared<DelayAction>(2.0) ;
			seq->pushAction(act) ;

			act = std::make_shared<SorterPowerAction>(*sorter, -0.5) ;
			seq->pushSubActionPair(sorter, act) ;

			act = std::make_shared<DelayAction>(2.0) ;
			seq->pushAction(act) ;

            act = std::make_shared<SingleMotorPowerAction>(*shooter, 0.5, 2.0) ;
			seq->pushSubActionPair(shooter, act) ;		

            act = std::make_shared<SingleMotorPowerAction>(*shooter, -0.5, 2.0) ;
			seq->pushSubActionPair(shooter, act) ;				

			act = std::make_shared<TankDriveTimedPowerAction>(*tankdrive, 0.5, 0.5, 2.0) ;
			seq->pushSubActionPair(tankdrive, act) ;

			act = std::make_shared<TankDriveTimedPowerAction>(*tankdrive, -0.5, -0.5, 2.0) ;
			seq->pushSubActionPair(tankdrive, act) ;			

            return seq ;          
        }        

		ActionSequencePtr BunnyAutoMode::createGameAutoMode(int number_of_crates) {
			ActionPtr act ;

            auto seq = std::make_shared<ActionSequence>(getRobot().getMessageLogger(), "StraightAndBackAutoMode") ;					

			auto &robot = getRobot() ;
			Bunny &bunny = dynamic_cast<Bunny &>(robot) ;

			auto intake = bunny.getBunnySubsystem()->getIntake() ;
			auto sorter = bunny.getBunnySubsystem()->getSorter() ;
			auto tankdrive = bunny.getBunnySubsystem()->getTankDrive() ;
			auto collector = bunny.getBunnySubsystem()->getCollector() ;
			auto hopper = bunny.getBunnySubsystem()->getHopper() ;
			auto shooter = bunny.getBunnySubsystem()->getShooter() ;

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
			seq->pushSubActionPair(intake, act)	;

			// Shooter to state stage

			act = std::make_shared<ShooterStageBallAction>(*shooter) ;
			seq->pushSubActionPair(shooter, act) ;
			
			// Drive straight (collecting as we go)

			act = std::make_shared<TankDriveDistanceAction>(*tankdrive, "automode:1:forward_distance") ;
			seq->pushSubActionPair(tankdrive, act) ;

			//
			// Drive back (ejecting as we go)
			// Note, we can create actions to associated with specific distances on the drive
			// So we create a sequence that is reused to trigger a shooter eject followed by
			// a shooter stage.  We attach this sequence at known intervals
			//
			// Pseudo Code:
			//     drvact = new drive straight action
			//     ActionSequencePtr actseq = new action sequence
			//     actseq->pushSubActionPair(shooter, eject)
			//     actseq->pushSubActionPair(shooter, stage)
			//
			//     std::vector<double> dists = calcDistanceForCrates()
			//     foreach(dist in dists)
			//       drvact->addTriggeredAction(dist, actseq)
			//     seq->pushSubActionPair(tankdrive, drvact)
			// 

			auto reverse_drive = std::make_shared<TankDriveDistanceAction>(*tankdrive, "automode:1:reverse_distance") ;
			seq->pushSubActionPair(tankdrive, reverse_drive) ;

			for(int i = 0; i < number_of_crates; i++){

				act = std::make_shared<ShooterEjectOneBallAction>(*shooter) ;
				reverse_drive->addTriggeredAction("automode:1:eject" + i, act) ;

				act = std::make_shared<ShooterStageBallAction>(*shooter) ;
				reverse_drive->addTriggeredAction("automode:1:eject" + i, act) ;

			}

			return seq ;
		}
    }
}