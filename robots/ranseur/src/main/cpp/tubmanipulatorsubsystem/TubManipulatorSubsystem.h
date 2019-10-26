#pragma once

#include "ranseuroi/RanseurOISubsystem.h"
#include "ranseurcameratracker/RanseurCameraTracker.h"
#include <tankdrive/TankDrive.h>
#include <RobotSubsystem.h>
#include "tubcollector/TubCollector.h"
#include "tubarm/TubArm.h"
#include "tubwrist/TubWrist.h"
#include <actions/Action.h>

namespace xero {
    namespace ranseur {

        class TubManipulatorSubsystem : public xero::base::RobotSubsystem {
        public:
            TubManipulatorSubsystem(xero::base::Robot &robot) ;
        
        std::shared_ptr<TubCollector> getTubCollector() {
            return tubcollector_ ;
            
        }

        std::shared_ptr<TubArm> getTub() {
            return tubarm_ ;
        }
        
        std::shared_ptr<TubWrist> getWrist() {
            return tubwrist_ ;
        }

        virtual bool canAcceptAction(xero::base::ActionPtr action) ;

        virtual std::string toString() ;
        
        //
        // Add gamemanipulator
        //
        // Add climber
        //
        // add other methods : 
        //

        std::shared_ptr<TubCollector> getTubCol() {
            return tubcollector_ ;
        }    

        ~TubManipulatorSubsystem() {
        }

        private:
        std::shared_ptr<TubCollector> tubcollector_ ;
        std::shared_ptr<TubArm> tubarm_ ;
        std::shared_ptr<TubWrist> tubwrist_ ;
        } ; 
    
    }
}
