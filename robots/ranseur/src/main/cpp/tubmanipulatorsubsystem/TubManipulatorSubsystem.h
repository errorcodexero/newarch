#pragma once

#include "ranseuroi/RanseurOISubsystem.h"
#include <tankdrive/TankDrive.h>
#include <RobotSubsystem.h>
#include "tubcollector/TubCollector.h"
#include "tubarm/TubArm.h"
#include "tubwrist/TubWrist.h"
#include <actions/Action.h>
#include "Subsystem.h"

#include "bunnyarm/BunnyArm.h"
#include <tubmanipulatorsubsystem/TubManipulatorSubsystem.h>

namespace xero {
    namespace ranseur {

        class TubManipulatorSubsystem : public xero::base::Subsystem {      
            ///"xero::base::Subsystem" -> derived from subsystem, not the robot (as was before Nov 13,2019) , b/c it isnt the "highest" subsytem -> that would be RanseurRobotSubsystem
        public:
            TubManipulatorSubsystem(xero::base::Subsystem* parent) ;
            virtual ~TubManipulatorSubsystem() {
            }

            std::shared_ptr<TubCollector> getTubCollector() {
                return tubcollector_ ;                
            }

            std::shared_ptr<TubArm> getTubArm() {
                return tubarm_ ;
            }
            
            std::shared_ptr<TubWrist> getTubWrist() {
                return tubwrist_ ;
            }

            virtual bool canAcceptAction(xero::base::ActionPtr action) ;

            virtual std::string toString() ;

        private:
            std::shared_ptr<TubCollector> tubcollector_ ;
            std::shared_ptr<TubArm> tubarm_ ;
            std::shared_ptr<TubWrist> tubwrist_ ;

        } ; 
    
    }
}
