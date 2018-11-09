#include "BunnySubsystem.h"
#include "bunnyoi/BunnyOISubsystem.h"

using namespace xero::base ;

namespace xero {
    namespace bunny2018 {
        BunnySubsystem::BunnySubsystem(Robot &robot) : Subsystem(robot, "bunny2018") {
			db_ = std::make_shared<TankDrive>(robot, std::list<int>{1, 2, 3}, std::list<int>{4, 5, 6}) ;
            db_->invertLeftMotors() ;
            db_->setEncoders(0, 1, 2, 3) ;
            db_->setGearShifter(0) ;
            db_->invertLeftEncoder() ;

            oi_ = std::make_shared<BunnyOISubsystem>(robot) ;

            addChild(db_) ;
            addChild(oi_) ;
        }

        BunnySubsystem::~BunnySubsystem() {
        }
    }
}
