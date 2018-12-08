#pragma once

#include "ShooterAction.h"
#include "Shooter.h"

namespace xero {
    namespace bunny2018 {
        class ShooterStageBallAction : public ShooterAction {

        public:
        
            ShooterStageBallAction(Shooter& shooter) ;

            virtual ~ShooterStageBallAction() ;

            virtual void start() ;
        
            virtual void run() ;
 
            virtual bool isDone() ;

            virtual void cancel() ;

            virtual std::string toString() ;
			
        };
    }
}