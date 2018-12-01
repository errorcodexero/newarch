#include <Robot.h>
#include "PhoenixSubsystem.h"

using namespace xero::base ;

namespace xero {
    namespace phoenix {
        PhoenixSubsystem::PhoenixSubsystem(Robot &robot) : Subsystem(robot, "phoenix") {
            auto& settings = robot.getSettingsParser() ;

            // Register message listener *before* any sub-system like the drive base which needs vision information.
            // The vision info received will only be present during 1 robot loop so ML's computeState()
            // must be called before other dependent sub-systems call their computeState() to access vision results.
            int socket_port_number = settings.getInteger("messagelistener:port") ;
            ml_ = std::make_shared<MessageListener>(robot, socket_port_number) ;
            addChild(ml_) ;                        

            db_ = std::make_shared<TankDrive>(robot, std::list<int>{1, 2, 3}, std::list<int>{4, 5, 6}) ;			
            addChild(db_) ;
                        
            wings_ = std::make_shared<Wings>(robot) ;
            addChild(wings_) ;
                        
            oi_ = std::make_shared<PhoenixOI>(robot) ;
            addChild(oi_) ;
                        
            liftcollect_ = std::make_shared<LiftingCollector>(robot) ;
            addChild(liftcollect_) ;
        }

        PhoenixSubsystem::~PhoenixSubsystem() {			
        }
    }
}
