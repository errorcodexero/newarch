#include "BunnySubsystem.h"
#include "bunnyoi/BunnyOISubsystem.h"

using namespace xero::base ;

namespace xero {
    namespace bunny2018 {
        BunnySubsystem::BunnySubsystem(Robot &robot) : Subsystem(robot, "bunny2018") {
            //
            // TODO: get the drive base hardware config from the parameters file
            //
			db_ = std::make_shared<TankDrive>(robot, std::list<int>{1, 2}, std::list<int>{3, 4}) ;
            db_->invertLeftMotors() ;
            db_->setEncoders(0, 1, 2, 3) ;
            db_->invertLeftEncoder() ;
            addChild(db_) ;

            oi_ = std::make_shared<BunnyOISubsystem>(robot) ;
            addChild(oi_) ;
            
            ml_ = std::make_shared<MessageListener>(robot) ;
            addChild(ml_) ;

            collector_ = std::make_shared<xero::base::SingleMotorSubsystem>(robot, "Collector", "hw:collector:motor") ;
            addChild(collector_) ;

            hopper_ = std::make_shared<xero::base::SingleMotorSubsystem>(robot, "Hopper", "hw:hopper:motor") ;
            addChild(hopper_) ;

            sorter_ = std::make_shared<Sorter>(robot) ;
            addChild(sorter_) ;     
        }

        BunnySubsystem::~BunnySubsystem() {
        }
    }
}
