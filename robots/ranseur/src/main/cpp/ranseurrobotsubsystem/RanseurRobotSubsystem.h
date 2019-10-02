#pragma once

#include "ranseuroi/RanseurOISubsystem.h"
#include "ranseurcameratracker/RanseurCameraTracker.h"
#include <tankdrive/TankDrive.h>
#include <RobotSubsystem.h>

namespace xero {
    namespace ranseur {

        //
        // Add gamemanipulator
        // Add climber
        //
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

            std::shared_ptr<RanseurCameraTracker> getCameraTracker() {
                return tracker_ ;
            }            

            virtual bool canAcceptAction(xero::base::ActionPtr action) ;
            
        private:
            std::shared_ptr<RanseurOISubsystem> oi_ ;
            std::shared_ptr<RanseurCameraTracker> tracker_ ;

        } ;
    }
}