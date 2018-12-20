#pragma once

#include "ShooterAction.h"
#include "Shooter.h"

namespace xero {
    namespace bunny2018 {

        /// \brief If a ball is staged, run belt at high speed for short period of time to eject it then stop.
        ///        If no ball is staged, run the belt at stage speed until the ball is detected, 
        ///        and then switch to higher speed temporarily to eject it. Then stop.
        class ShooterEjectOneBallAction : public ShooterAction {

        public:
        
            ShooterEjectOneBallAction(Shooter& shooter) ;

            virtual ~ShooterEjectOneBallAction() ;

            virtual void start() ;
        
            virtual void run() ;
 
            virtual bool isDone() ;

            virtual void cancel() ;

            virtual std::string toString() ;
			
        private:
            enum class State {
                NOT_STARTED,
                STAGING,
                EJECTING,
                DONE
            };
            State state_;
            double eject_start_time_;

            double getTime();
        };
    }
}
