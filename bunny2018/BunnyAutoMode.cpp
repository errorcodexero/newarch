#include "BunnyAutoMode.h"
#include "BunnySubsystem.h"
#include "Bunny.h"
#include "OISubsystem.h"
#include "BunnyOISubsystem.h"
#include <tankdrive/TankDriveDistanceAction.h>
#include <tankdrive/TankDriveCharAction.h>
#include <tankdrive/TankDriveAngleCharAction.h>
#include <tankdrive/TankDriveAngleAction.h>
#include <SmartDashboard/SmartDashboard.h>

using namespace xero::base ;

namespace xero {
    namespace bunny2018 {
        BunnyAutoMode::BunnyAutoMode(Robot &robot) : AutoController(robot) {
            createAutoMode() ;
        }
        
        BunnyAutoMode::~BunnyAutoMode() {

        }

        void BunnyAutoMode::createAutoMode() {
            Bunny &bunny = dynamic_cast<Bunny &>(getRobot()) ;
            auto oi = bunny.getBunnySubsystem()->getOI() ;
            int sel = oi->getAutoModeSelector() ;

			//
			// If there is no hardware to set the automode, default to auto
			// mode zero
			//
			if (sel == -1)
				sel = 0 ;

			std::string mode = std::to_string(sel) ;
			frc::SmartDashboard::PutString("automode:", mode) ;

            auto seq = std::make_shared<ActionSequence>(getRobot().getMessageLogger()) ;
            auto tankdrive = std::dynamic_pointer_cast<TankDrive>(getRobot().getDriveBase()) ;

            //auto act = std::make_shared<TankDriveDistanceAction>(*tankdrive, 100.0) ;
            auto act = std::make_shared<TankDriveAngleAction>(*tankdrive, 90.0) ;            
            //auto act = std::make_shared<TankDriveAngleCharAction>(*tankdrive, 5.0, 0.6) ;
            //auto act = std::make_shared<TankDriveCharAction>(*tankdrive, 5.0, 0.6) ;
            seq->pushSubActionPair(tankdrive, act) ;

            setAction(seq) ;
        }
    }
}