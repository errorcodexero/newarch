#pragma once

#include "WingsAction.h"

namespace xero {
    namespace phoenix {
        class WingsDeployAction : public WingsAction {
        public:
            WingsDeployAction(Wings &wings) : WingsAction(wings) {                
            }

            virtual ~WingsDeployAction() {                
            }

            bool getDeploy() {
                return true ;
            }

            virtual void start() {                
            }

            virtual void end() {                
            }

            virtual void run() {                
            }

            virtual bool isDone() {
                return true ;
            }

            virtual bool cancel() {
                return true ;
            }

            virtual std::string toString() {
                return action_name ;
            }

        private:
            static std::string action_name ;
        } ;
    }
}
