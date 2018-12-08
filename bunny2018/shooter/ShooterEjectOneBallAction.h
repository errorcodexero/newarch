#pragma once

#include "ShooterAction.h"
#include "Shooter.h"

namespace xero {
    namespace bunny2018 {
        class ShooterEjectOneBallAction : public ShooterAction {

        public:
        
            ShooterEjectOneBallAction(Shooter& shooter) ;

            virtual ~ShooterEjectOneBallAction() ;

            virtual void start() ;
        
            virtual void run() ;
 
            virtual bool isDone() ;

            virtual void cancel() ;

            virtual std::string toString() ;
			
        };
    }
}
