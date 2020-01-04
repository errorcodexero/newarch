#pragma once

#include "droidoi/droidOISubsystem.h"
#include <tankdrive/TankDrive.h>
#include <RobotSubsystem.h>

namespace xero {
    namespace droid {

        //
        // Add gamemanipulator
        // Add climber
        //
        class droidSubsystem : public xero::base::RobotSubsystem {
        public:
            droidSubsystem(xero::base::Robot &robot) ;
            virtual ~droidSubsystem() ;

            std::shared_ptr<xero::base::TankDrive> getTankDrive() {
                return std::dynamic_pointer_cast<xero::base::TankDrive>(getDriveBase()) ;
            }
          
            std::shared_ptr<droidOISubsystem> getOI() {
                return oi_ ;
            }    

            virtual bool canAcceptAction(xero::base::ActionPtr action) ;
            
        private:
            ////%% Start DefineChildSubsystems        
            std::shared_ptr<droidOISubsystem> oi_ ;
            ////%% End DefineChildSubsystems
        } ;
    }
}
