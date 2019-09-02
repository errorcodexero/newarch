#pragma once

#include "ShooterAction.h"
#include "Shooter.h"

namespace xero {
    namespace bunny2018 {

        /// \brief Run the belt at reduced speed until a ball is detected by the sensor, then stop the belt (motor).
        class ShooterStageBallAction : public ShooterAction {

        public:
        
            ShooterStageBallAction(Shooter& shooter) ;

            virtual ~ShooterStageBallAction() ;

            virtual void start() ;
        
            virtual void run() ;
 
            virtual bool isDone() ;

            virtual void cancel() ;

            virtual std::string toString() ;

        private:
        
            bool is_done_ ;
            
        };
    }
}