#pragma once

#include <actions/Action.h>

namespace xero {
    namespace phoenix {
        class Grabber ;

        class GrabberAction : public xero::base::Action {
        public:
            GrabberAction(Grabber &grabber) : grabber_(grabber) {
            }

        protected:
            Grabber &getGrabber() {
                return grabber_ ;
            }

            const Grabber &getGrabber() const {
                return grabber_ ;
            }
            
        private:
            Grabber &grabber_ ;
        } ;
    }
}
