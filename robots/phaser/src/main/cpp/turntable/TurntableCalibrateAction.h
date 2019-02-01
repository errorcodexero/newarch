#pragma once

#include "TurntableAction.h"
#include "Turntable.h"

namespace xero {
    namespace phaser {
        class TurntableCalibrateAction : public TurntableAction {
        public:
            TurntableCalibrateAction(Turntable &Turntable) ;
            virtual ~TurntableCalibrateAction() ;

            virtual void start() {
                getTurntable().calibrate() ;
            }

            virtual void run() {                
            }

            virtual bool isDone() {
                return true ;
            }

            virtual void cancel() {
            }

            virtual std::string toString() {
                return action_name ;
            }

        private:
            static std::string action_name ;
        } ;
    }
}
