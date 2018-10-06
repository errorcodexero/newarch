#include "PhoenixSubsystem.h"

using namespace xero::base ;

namespace xero {
	namespace phoenix {
		PhoenixSubsystem::PhoenixSubsystem(Robot &robot) : Subsystem(robot, "phoenix") {
			db_ = std::make_shared<TankDrive>(robot, std::list<int>{1, 2, 3}, std::list<int>{4, 5, 6}) ;			
			wings_ = std::make_shared<Wings>(robot) ;
			liftcollect_ = std::make_shared<LiftingCollector>(robot) ;
			oi_ = std::make_shared<PhoenixOI>(robot) ;
		}

		PhoenixSubsystem::~PhoenixSubsystem() {			
		}
	}
}
