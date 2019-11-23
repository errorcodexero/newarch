#pragma once

#include "ranseuroi/RanseurOISubsystem.h"
#include "ranseurcameratracker/RanseurLimeLight.h"
#include "bunnyarm/BunnyArm.h"
#include <tankdrive/TankDrive.h>
#include <RobotSubsystem.h>
#include <tubmanipulatorsubsystem/TubManipulatorSubsystem.h>

namespace xero {
    namespace ranseur {
        class RanseurRobotSubsystem : public xero::base::RobotSubsystem {
        public:     
            RanseurRobotSubsystem(xero::base::Robot &robot) ;
            virtual ~RanseurRobotSubsystem() ;

            std::shared_ptr<xero::base::TankDrive> getTankDrive() {
                return std::dynamic_pointer_cast<xero::base::TankDrive>(getDriveBase()) ;
            }
          
            std::shared_ptr<RanseurOISubsystem> getOI() {
                return oi_ ;
            }    

            std::shared_ptr<RanseurLimeLight> getCameraTracker() {
                return tracker_ ;
            }            

            std::shared_ptr<BunnyArm> getBunnyArm() {
                return bunnyArm_;
            }

             std::shared_ptr<TubManipulatorSubsystem> getTubManipulatorSubsystem() {
                return manipulator_ ;
            }    

            virtual bool canAcceptAction(xero::base::ActionPtr action) ;
            
        private:
            std::shared_ptr<RanseurOISubsystem> oi_ ;
            std::shared_ptr<RanseurLimeLight> tracker_ ;
            std::shared_ptr<BunnyArm> bunnyArm_;
            std::shared_ptr<TubManipulatorSubsystem> manipulator_ ;
        } ;
    }
}
