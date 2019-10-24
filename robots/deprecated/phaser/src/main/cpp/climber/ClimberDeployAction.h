#pragma once

#include "ClimberAction.h"
#include "Climber.h"

namespace xero {
    namespace phaser {
        class ClimberDeployAction : public ClimberAction {
        public:
            ClimberDeployAction(Climber &climber) : ClimberAction(climber) { 
                is_done_ = false;               
            }
            
            virtual ~ClimberDeployAction() {                
            }

            virtual void start() {                
                is_done_ = false ;
            }

            virtual void run() {
                if (!is_done_) {
                    Climber &climber = getClimber() ;
                    climber.solenoid_->Set(true);
                    is_done_ = true ;
                }
            }

            virtual bool isDone() {
                return is_done_ ;
            }

            virtual void cancel() {
                is_done_ = true ;
            }

            virtual std::string toString() {
                return action_name ;
            }

        private:
            bool is_done_ ;
            static const std::string action_name ;
        } ;
    }
}
