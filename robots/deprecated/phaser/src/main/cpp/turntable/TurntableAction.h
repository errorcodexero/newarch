#pragma once

#include <Action.h>

namespace xero {
    namespace phaser {
        class  Turntable;

        class TurntableAction : public xero::base::Action {
        public:
            TurntableAction(Turntable &turntable) : turntable_(turntable) {
            }

        protected:
            Turntable &getTurntable() {
                return turntable_ ;
            }

            const Turntable &getTurntable() const {
                return turntable_ ;
            }
            
        private:
            Turntable &turntable_ ;
        } ;
    }
}