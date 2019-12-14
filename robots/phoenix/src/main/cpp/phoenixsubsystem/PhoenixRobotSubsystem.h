#pragma once

#include <tankdrive/TankDrive.h>
#include <Subsystem.h>
#include <RobotSubsystem.h>

namespace xero {
    namespace phoenix {
        class LiftingCollector ;
        class PhoenixOISubsystem ;
        class Wings ;

        class PhoenixRobotSubsystem : public xero::base::RobotSubsystem {
        public:
            PhoenixRobotSubsystem(xero::base::Robot &robot) ;
            virtual ~PhoenixRobotSubsystem() ;

            virtual bool canAcceptAction(xero::base::ActionPtr action) ;            

            std::shared_ptr<PhoenixOISubsystem> getOI() { 
                return oi_ ;
            }

            std::shared_ptr<Wings> getWings() {
                return wings_ ;
            }

            std::shared_ptr<LiftingCollector> getLiftingCollector() {
                return lifting_collector_ ;
            }

            std::shared_ptr<xero::base::TankDrive> getTankDrive() {
                return db_ ;
            }
            
        private:
            std::shared_ptr<PhoenixOISubsystem> oi_ ;
            std::shared_ptr<Wings> wings_ ;
            std::shared_ptr<LiftingCollector> lifting_collector_ ;
            std::shared_ptr<xero::base::TankDrive> db_ ;
        } ;
    }
}