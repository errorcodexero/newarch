#pragma once

#include "ShooterStageBallAction.h"
#include "Shooter.h"

namespace xero {
    namespace bunny2018 {

        /// \brief If a ball is staged, run belt at high speed for short period of time to eject it then stop.
        ///        If no ball is staged, run the belt at stage speed until the ball is detected, 
        ///        and then switch to higher speed temporarily to eject it. Then stop.
        class ShooterEjectOneBallAction : public ShooterStageBallAction {

        public:
        
            ShooterEjectOneBallAction(Shooter& shooter) ;

            virtual ~ShooterEjectOneBallAction() ;

            virtual void start() ;
        
            virtual void run() ;
 
            virtual bool isDone() ;

            virtual void cancel() ;

            virtual std::string toString() ;

        protected:
            virtual bool ejectTrigger() {
                return true ;
            }

            virtual bool repeatEjectOne() {
                return false ;
            }

        private:
            void readyStaging() ;
            void readyEjecting() ;
			
        private:
            enum class State {
                NOT_STARTED,
                STAGING,
                WAITING,
                EJECTING,
                DONE
            };
            State state_;
            double eject_start_time_;

            double getTime();
        };
    }
}
