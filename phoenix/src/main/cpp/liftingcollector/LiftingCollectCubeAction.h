#pragma once

#include "LiftingCollectorAction.h"

namespace xero {
    namespace phoenix {
        class LiftingCollectCubeAction : public LiftingCollectorAction {
        public:
            LiftingCollectCubeAction(LiftingCollector &col, double height) ;
            LiftingCollectCubeAction(LiftingCollector &col, const std::string &height) ;
            virtual ~LiftingCollectCubeAction() ;

            virtual void start() ;
            virtual void run() ;
            virtual bool isDone()  ;
            virtual void cancel() ;
            virtual std::string toString() {
                return action_name ;
            }

        private:
            static std::string action_name ;                    

        private:
            double drive_height_ ;
            xero::base::ActionPtr collect_action_ ;
        } ;
    }
}