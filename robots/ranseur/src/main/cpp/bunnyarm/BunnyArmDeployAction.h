#pragma once

#include "BunnyArmAction.h"
#include "BunnyArm.h"

namespace xero {
    namespace ranseur {
        class BunnyArmDeployAction : public BunnyArmAction {
        public:
            BunnyArmDeployAction(BunnyArm &bunnyArm) : BunnyArmAction(bunnyArm) { 
                is_done_ = false;               
            }
            
            virtual ~BunnyArmDeployAction() {                
            }

            virtual void start() {                
                is_done_ = false ;
            }

            virtual void run() {
                if (!is_done_) {
                    BunnyArm &bunnyArm = getBunnyArm() ;
                    bunnyArm.solenoid_->Set(true);
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
