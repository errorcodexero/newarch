#include "PhoenixRobotSubsystem.h"
#include "liftingcollector/LiftingCollector.h"
#include "wings/Wings.h"
#include "phoenixoi/PhoenixOISubsystem.h"
#include "PhoenixRobotSubsystemAction.h"
#include "phoenixgroups.h"
#include <tankdrive/TankDrive.h>
#include <Robot.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace phoenix {
        PhoenixRobotSubsystem::PhoenixRobotSubsystem(Robot &robot) : RobotSubsystem(robot, "PhoenixRobotSubsystem") {

            //
            // Add a tank drive.  The exact details are defined in the params file.
            //
            addTankDrive() ;

            //
            // Create the wings subsystem
            //
            wings_ = std::make_shared<Wings>(this) ;
            addChild(wings_) ;

            //
            // Create the lifting collector subsystem
            //
            lifting_collector_ = std::make_shared<LiftingCollector>(this) ;
            addChild(lifting_collector_) ;
            lifting_collector_->createNamedSequences() ;

            //
            // Create the Phoenix OI subsystem.  It is created last as it may
            // refer to the other subsystems
            //
            oi_ = std::make_shared<PhoenixOISubsystem>(this) ;
            addChild(oi_) ;         
        }

        PhoenixRobotSubsystem::~PhoenixRobotSubsystem() {
            oi_ = nullptr ;
            wings_ = nullptr ;
            lifting_collector_ = nullptr ;
        }

        bool PhoenixRobotSubsystem::canAcceptAction(xero::base::ActionPtr action) {
            auto ptr = std::dynamic_pointer_cast<PhoenixRobotSubsystemAction>(action) ;
            return ptr != nullptr ;
        }
    }
}
