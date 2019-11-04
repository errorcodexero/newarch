#pragma once

#include "$$robotname$$oi/$$RobotName$$OISubsystem.h"
#include <tankdrive/TankDrive.h>
#include <RobotSubsystem.h>

namespace xero {
    namespace $$robotname$$ {

        //
        // Add gamemanipulator
        // Add climber
        //
        class $$RobotName$$Subsystem : public xero::base::RobotSubsystem {
        public:
            $$RobotName$$Subsystem(xero::base::Robot &robot) ;
            virtual ~$$RobotName$$Subsystem() ;

            std::shared_ptr<xero::base::TankDrive> getTankDrive() {
                return std::dynamic_pointer_cast<xero::base::TankDrive>(getDriveBase()) ;
            }
          
            std::shared_ptr<$$RobotName$$OISubsystem> getOI() {
                return oi_ ;
            }    

            virtual bool canAcceptAction(xero::base::ActionPtr action) ;
            
        private:
            std::shared_ptr<$$RobotName$$OISubsystem> oi_ ;
        } ;
    }
}