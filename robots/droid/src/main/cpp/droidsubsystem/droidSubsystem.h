#pragma once

#include "droidoi/DroidOISubsystem.h"
#include <tankdrive/TankDrive.h>
#include <RobotSubsystem.h>

namespace xero {
    namespace droid {

        //
        // Add gamemanipulator
        // Add climber
        //
        class DroidSubsystem : public xero::base::RobotSubsystem {
        public:
            DroidSubsystem(xero::base::Robot &robot) ;
            virtual ~DroidSubsystem() ;

            std::shared_ptr<xero::base::TankDrive> getTankDrive() {
                return std::dynamic_pointer_cast<xero::base::TankDrive>(getDriveBase()) ;
            }
          
            std::shared_ptr<DroidOISubsystem> getOI() {
                return oi_ ;
            }    

            virtual bool canAcceptAction(xero::base::ActionPtr action) ;
            
        private:
            ////%% Start DefineChildSubsystems        
            std::shared_ptr<DroidOISubsystem> oi_ ;
            ////%% End DefineChildSubsystems
        } ;
    }
}
