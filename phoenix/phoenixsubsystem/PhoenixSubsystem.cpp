#include "PhoenixSubsystem.h"

using namespace xero::base ;

namespace xero {
    namespace phoenix {
        PhoenixSubsystem::PhoenixSubsystem(Robot &robot) : Subsystem(robot, "phoenix") {
            db_ = std::make_shared<TankDrive>(robot, std::list<int>{1, 2, 3}, std::list<int>{4, 5, 6}) ;			
            addChild(db_) ;
                        
            wings_ = std::make_shared<Wings>(robot) ;
            addChild(wings_) ;
                        
            oi_ = std::make_shared<PhoenixOI>(robot) ;
            addChild(oi_) ;
                        
            liftcollect_ = std::make_shared<LiftingCollector>(robot) ;
            addChild(liftcollect_) ;
                        
            ml_ = std::make_shared<MessageListener>(robot) ;
            addChild(ml_) ;                        
        }

        PhoenixSubsystem::~PhoenixSubsystem() {			
        }
    }
}
