#include "Bunny.h"
#include "automodes/BunnyAutoMode.h"
#include "bunnysubsystem/BunnySubsystem.h"
#include "bunnyoi/BunnyOISubsystem.h"
#include <tankdrive/TankDriveDistanceAction.h>
#include <tankdrive/TankDriveCharAction.h>
#include <tankdrive/TankDriveAngleCharAction.h>
#include <tankdrive/TankDriveAngleAction.h>

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
			if (sel == -1) {
				sel = 0 ;
			}

			if (sel == 10)
				sel = 0 ;

			sel = 0 ;

			std::string autostr = std::to_string(sel) ;
			frc::SmartDashboard::PutString("AutoMode", autostr) ;				

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

				default:
					mode = nullptr ;
					break ;
			}

			if (mode == nullptr)
				name_ = "None" ;
			else
				name_ = mode->getName() ;

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
    }
}