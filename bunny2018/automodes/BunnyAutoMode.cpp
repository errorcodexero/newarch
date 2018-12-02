#include "Bunny.h"
#include "automodes/BunnyAutoMode.h"
#include "bunnysubsystem/BunnySubsystem.h"
#include "bunnyoi/BunnyOISubsystem.h"
#include <tankdrive/TankDriveDistanceAction.h>
#include <tankdrive/TankDriveCharAction.h>
#include <tankdrive/TankDriveAngleCharAction.h>
#include <tankdrive/TankDriveAngleAction.h>
#include <singlemotorsubsystem/SingleMotorVoltageAction.h>
#include <sorter/SorterDutyCycleAction.h>
#include <sorter/SorterCalibrateAction.h>
#include <sorter/SorterStageBallAction.h>
#include <sorter/SorterEjectAction.h>
#include <sorter/SorterRotateAngleAction.h>
#include <DelayAction.h>
#include <ParallelAction.h>

using namespace xero::base ;

namespace xero {
    namespace bunny2018 {
        BunnyAutoMode::BunnyAutoMode(Robot &robot) : AutoController(robot) {
            createAutoMode() ;
        }
        
        BunnyAutoMode::~BunnyAutoMode() {

        }

		std::string BunnyAutoMode::getControllerInformation() {
			return name_ ;
		}

        void BunnyAutoMode::createAutoMode() {
            Bunny &bunny = dynamic_cast<Bunny &>(getRobot()) ;
            auto oi = bunny.getBunnySubsystem()->getOI() ;
            int sel = oi->getAutoModeSelector() ;
            ActionSequencePtr mode ;

			//
			// If there is no hardware to set the automode, default to auto
			// mode zero
			//
			if (sel < 0 || sel > 9)
				sel = 0 ;
			
			sel = 8 ;

			switch(sel) {
				case 0:
					mode = createAutoModeZero() ;
					break ;

				case 1:
					mode = createAutoModeOne() ;
					break ;

				case 2:
					mode = createAutoModeTwo() ;
					break ;

				case 3:
					mode = createAutoModeThree() ;
					break ;

				case 4:
					mode = createAutoModeFour() ;
					break ;

				case 5:
					mode = createAutoModeFive() ;
					break ;

				case 6:
					mode = createAutoModeSix() ;
					break ;

				case 7:
					mode = createAutoModeSeven() ;
					break ;		

				case 8:
					mode = createAutoModeEight() ;
					break ;				

				case 9:
					mode = createAutoModeNine() ;
					break ;			

				case 10:
					mode = createRotateSorterAutoMode() ;
					break ;																					

				default:
					mode = nullptr ;
					break ;
			}

			if (mode == nullptr)
				name_ = "None" ;
			else
				name_ = mode->getName() ;

			std::string autostr = std::to_string(sel) ;
			frc::SmartDashboard::PutString("AutoModeNumber", autostr) ;
			frc::SmartDashboard::PutString("AutoModeName", name_) ;		

            setAction(mode) ;
        }

		ActionSequencePtr BunnyAutoMode::createAutoModeZero() {
            auto tankdrive = std::dynamic_pointer_cast<TankDrive>(getRobot().getDriveBase()) ;
            auto seq = std::make_shared<ActionSequence>(getRobot().getMessageLogger(), "TankDriveStraightChar") ;
            auto act = std::make_shared<TankDriveCharAction>(*tankdrive, 10.0, 1.0) ;
			seq->pushSubActionPair(tankdrive, act) ;
			return seq ;

		}

		ActionSequencePtr BunnyAutoMode::createAutoModeOne() {
            auto tankdrive = std::dynamic_pointer_cast<TankDrive>(getRobot().getDriveBase()) ;

            auto seq = std::make_shared<ActionSequence>(getRobot().getMessageLogger(), "TankDriveRotateChar") ;
            auto act = std::make_shared<TankDriveAngleCharAction>(*tankdrive, 5.0, 0.0, 1.0, 0.1) ;			
			seq->pushSubActionPair(tankdrive, act) ;

			return seq ;
		}

		ActionSequencePtr BunnyAutoMode::createAutoModeTwo() {
            auto tankdrive = std::dynamic_pointer_cast<TankDrive>(getRobot().getDriveBase()) ;

            auto seq = std::make_shared<ActionSequence>(getRobot().getMessageLogger(), "TankDriveStraight") ;
            auto act = std::make_shared<TankDriveDistanceAction>(*tankdrive, 48.0) ;			
			seq->pushSubActionPair(tankdrive, act) ;

			return seq ;
		}

		ActionSequencePtr BunnyAutoMode::createAutoModeThree() {
            auto tankdrive = std::dynamic_pointer_cast<TankDrive>(getRobot().getDriveBase()) ;

            auto seq = std::make_shared<ActionSequence>(getRobot().getMessageLogger(), "TankDriveRotate") ;
            auto act = std::make_shared<TankDriveAngleAction>(*tankdrive, 90.0) ;
			seq->pushSubActionPair(tankdrive, act) ;

			return seq ;
		}

		ActionSequencePtr BunnyAutoMode::createAutoModeFour() {
            auto tankdrive = std::dynamic_pointer_cast<TankDrive>(getRobot().getDriveBase()) ;

            auto seq = std::make_shared<ActionSequence>(getRobot().getMessageLogger(), "TankDriveRotate") ;
            auto act = std::make_shared<TankDriveAngleAction>(*tankdrive, -90.0) ;
			seq->pushSubActionPair(tankdrive, act) ;

			return seq ;
		}		

		ActionSequencePtr BunnyAutoMode::createAutoModeFive() {
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

		ActionSequencePtr BunnyAutoMode::createAutoModeSix() {
			xero::base::ActionPtr act ;
			auto &robot = getRobot() ;
			Bunny &bunny = dynamic_cast<Bunny &>(robot) ;
			auto collector = bunny.getBunnySubsystem()->getCollector() ;
            auto seq = std::make_shared<ActionSequence>(getRobot().getMessageLogger(), "CollectorTest") ;
            act = std::make_shared<SingleMotorVoltageAction>(*collector, 0.2) ;
			seq->pushSubActionPair(collector, act) ;

			act = std::make_shared<DelayAction>(5.0) ;
			seq->pushAction(act) ;

            act = std::make_shared<SingleMotorVoltageAction>(*collector, 0) ;
			seq->pushSubActionPair(collector, act) ;			
			return seq ;
		}	

		ActionSequencePtr BunnyAutoMode::createAutoModeSeven() {
			xero::base::ActionPtr act ;			
			auto &robot = getRobot() ;
			Bunny &bunny = dynamic_cast<Bunny &>(robot) ;
			auto hopper = bunny.getBunnySubsystem()->getHopper() ;
            auto seq = std::make_shared<ActionSequence>(getRobot().getMessageLogger(), "HopperTest") ;

            act = std::make_shared<SingleMotorVoltageAction>(*hopper, 0.2) ;
			seq->pushSubActionPair(hopper, act) ;

			act = std::make_shared<DelayAction>(5.0) ;
			seq->pushAction(act) ;

            act = std::make_shared<SingleMotorVoltageAction>(*hopper, 0) ;
			seq->pushSubActionPair(hopper, act) ;

			return seq ;
		}	

		ActionSequencePtr BunnyAutoMode::createAutoModeEight() {
			xero::base::ActionPtr act ;
			auto &robot = getRobot() ;
			Bunny &bunny = dynamic_cast<Bunny &>(robot) ;
			auto sorter = bunny.getBunnySubsystem()->getSorter() ;
            auto seq = std::make_shared<ActionSequence>(getRobot().getMessageLogger(), "SorterMotorTest") ;

            act = std::make_shared<SorterStageBallAction>(*sorter, Sorter::BallColor::Red) ;
			seq->pushSubActionPair(sorter, act) ;			

			return seq ;
		}

		ActionSequencePtr BunnyAutoMode::createAutoModeNine() {
			xero::base::ActionPtr act ;

			auto &robot = getRobot() ;
			Bunny &bunny = dynamic_cast<Bunny &>(robot) ;

			auto sorter = bunny.getBunnySubsystem()->getSorter() ;
            auto tankdrive = std::dynamic_pointer_cast<TankDrive>(getRobot().getDriveBase()) ;
			auto collector = bunny.getBunnySubsystem()->getCollector() ;
			auto hopper = bunny.getBunnySubsystem()->getHopper() ;

            auto seq = std::make_shared<ActionSequence>(getRobot().getMessageLogger(), "AutoMode One") ;
			auto seq2 = std::make_shared<ActionSequence>(getRobot().getMessageLogger(), "Calibrate/Stage") ;
			std::shared_ptr<ParallelAction> parallel ;

            act = std::make_shared<SingleMotorVoltageAction>(*hopper, "automode:1:hopper_power") ;
			seq->pushSubActionPair(hopper, act) ;

            act = std::make_shared<SingleMotorVoltageAction>(*collector, "automode:1:collector_power") ;
			seq->pushSubActionPair(collector, act) ;				

			act = std::make_shared<TankDriveDistanceAction>(*tankdrive, "automode:1:forward_distance") ;
			parallel->addAction(act) ;

            act = std::make_shared<SorterCalibrateAction>(*sorter) ;
			seq2->pushSubActionPair(sorter, act) ;

            act = std::make_shared<SorterStageBallAction>(*sorter, Sorter::BallColor::Red) ;
			seq2->pushSubActionPair(sorter, act) ;

			parallel->addAction(seq2) ;

			seq->pushAction(parallel) ;

			auto dract = std::make_shared<TankDriveDistanceAction>(*tankdrive, "automode:1:reverse_distance") ;
			parallel->addAction(act) ;

			seq2 = std::make_shared<ActionSequence>(getRobot().getMessageLogger(), "Eject/Stage") ;
			act = std::make_shared<SorterEjectAction>(*sorter) ;
			seq2->pushSubActionPair(sorter, act) ;
            act = std::make_shared<SorterStageBallAction>(*sorter, Sorter::BallColor::Red) ;
			seq2->pushSubActionPair(sorter, act) ;

			dract->addTriggeredAction("automode:1:eject1", seq2) ;
			dract->addTriggeredAction("automode:1:eject2", seq2) ;
			dract->addTriggeredAction("automode:1:eject3", seq2) ;
			dract->addTriggeredAction("automode:1:eject4", seq2) ;
			dract->addTriggeredAction("automode:1:eject5", seq2) ;
			dract->addTriggeredAction("automode:1:eject6", seq2) ;
			dract->addTriggeredAction("automode:1:eject7", seq2) ;
			dract->addTriggeredAction("automode:1:eject8", seq2) ;
			dract->addTriggeredAction("automode:1:eject9", seq2) ;
			dract->addTriggeredAction("automode:1:eject10", seq2) ;
			dract->addTriggeredAction("automode:1:eject11", seq2) ;
			dract->addTriggeredAction("automode:1:eject12", seq2) ;
			dract->addTriggeredAction("automode:1:eject13", seq2) ;																																				
			dract->addTriggeredAction("automode:1:eject14", seq2) ;	
			dract->addTriggeredAction("automode:1:eject15", seq2) ;

			return seq ;
		}


		ActionSequencePtr BunnyAutoMode::createRotateSorterAutoMode() {
			xero::base::ActionPtr act ;
			auto &robot = getRobot() ;
			Bunny &bunny = dynamic_cast<Bunny &>(robot) ;
			auto sorter = bunny.getBunnySubsystem()->getSorter() ;
            auto seq = std::make_shared<ActionSequence>(getRobot().getMessageLogger(), "SorterMotorTest") ;

            act = std::make_shared<SorterRotateAngleAction>(*sorter, 720, 0.3) ;
			seq->pushSubActionPair(sorter, act) ;			

			return seq ;
		}
    }
}