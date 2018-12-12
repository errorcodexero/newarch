#include "Bunny.h"
#include "automodes/BunnyAutoMode.h"
#include "bunnysubsystem/BunnySubsystem.h"
#include "bunnyoi/BunnyOISubsystem.h"
#include <tankdrive/TankDriveDistanceAction.h>
#include <tankdrive/TankDriveCharAction.h>
#include <tankdrive/TankDriveAngleCharAction.h>
#include <tankdrive/TankDriveAngleAction.h>
#include <singlemotorsubsystem/SingleMotorVoltageAction.h>
#include <sorter/SorterPowerAction.h>
#include <sorter/SorterCalibrateAction.h>
#include <sorter/SorterRotateAngleAction.h>
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

            sel = -1 ;

			switch(sel) {
				case 0:
					mode = createStraightBackAutomode() ;
					break ;

				case 1:
					mode = createRotateChar() ;
					break ;

				case 2:
					mode = createDriveStraightTest() ;
					break ;

				case 3:
					mode = createRotatePos90Test() ;
					break ;

				case 4:
					mode = createRotateNeg90Test() ;
					break ;

				case 5:
					mode = createDriveSquareTest() ;
					break ;

				case 6:
					mode = createDriveStraightChar() ;				
					break ;

				case 7:
					mode = createStraightBackAutomode() ;
					break ;		

				case 8:
					mode = createRotateSorterAutoMode() ;
					break ;				

				case 9:
					mode = createGameAutoMode() ;
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

		ActionSequencePtr BunnyAutoMode::createGameAutoMode() {
			ActionPtr act ;
            auto seq = std::make_shared<ActionSequence>(getRobot().getMessageLogger(), "StraightAndBackAutoMode") ;		
			
            auto tankdrive = std::dynamic_pointer_cast<TankDrive>(getRobot().getDriveBase()) ;

			// Eject existing ball

			// Collector to state on

			// Hopper to state on

			// Shooter to state stage
			
			// Drive straight (collecting as we go)

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

		}
    }
}