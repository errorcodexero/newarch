#include "BunnySubsystem.h"
#include "BunnyOISubsystem.h"

using namespace xero::base ;

namespace xero {
    namespace bunny2018 {
        BunnySubsystem::BunnySubsystem(Robot &robot) : Subsystem(robot, "bunny2018") {
			db_ = std::make_shared<TankDrive>(robot, std::list<int>{1, 2, 3}, std::list<int>{4, 5, 6}) ;
            db_->invertRightMotors() ;
            db_->setEncoders(0, 1, 2, 3) ;

            oi_ = std::make_shared<BunnyOISubsystem>(robot) ;

            addChild(db_) ;
            addChild(oi_) ;
        }

        BunnySubsystem::~BunnySubsystem() {
        }
    }
}
