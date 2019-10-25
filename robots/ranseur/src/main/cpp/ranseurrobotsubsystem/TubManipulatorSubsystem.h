#pragma once

#include "ranseuroi/RanseurOISubsystem.h"
#include "ranseurcameratracker/RanseurCameraTracker.h"
#include <tankdrive/TankDrive.h>
#include <RobotSubsystem.h>
#include <TubIntake/Collector.h>          // because we need to access the name of the collector
#include "Action.h"

namespace xero {
    namespace ranseur {

        class TubManipulatorSubsystem : public xero::base::RobotSubsystem {
        public:
            TubManipulatorSubsystem(xero::base::Robot &robot) ;
        
        std::shared_ptr<Collector> getCollector() {
            return collector_ ;
        }
        
        virtual bool canAcceptAction(xero::base::ActionPtr action) ;

        virtual void reset() ;                  

//        virtual void init(LoopType ltype) ;  
            
        virtual void run() ;

        virtual void cancelAction() ;    
      
        virtual bool setAction(xero::base::ActionPtr action, bool force = false) ;

        virtual void computeState() ;

        virtual std::string toString() ;
        
        //
        // Add gamemanipulator
        //
        // Add climber
        //
        // add other methods : 
        //

        std::shared_ptr<Collector> getCol() {
            return collector_ ;
        }    

        ~TubManipulatorSubsystem() {
        }

        private:
        std::shared_ptr<Collector> collector_ ;

        } ; 
    
    }
}
