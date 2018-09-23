#include "BunnyAutoMode.h"
#include "BunnySubsystem.h"
#include "Bunny.h"
#include "OISubsystem.h"
#include "BunnyOISubsystem.h"
#include <tankdrive/TankDriveDistanceAction.h>

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

            auto seq = std::make_shared<ActionSequence>(getRobot().getMessageLogger()) ;
            auto tankdrive = std::dynamic_pointer_cast<TankDrive>(getRobot().getDriveBase()) ;

            if (sel >= 0 && sel <=10) {
                auto act = std::make_shared<TankDriveDistanceAction>(*tankdrive, 100.0) ;
                seq->pushSubActionPair(tankdrive, act) ;
            }

            setAction(seq) ;
        }
    }
}