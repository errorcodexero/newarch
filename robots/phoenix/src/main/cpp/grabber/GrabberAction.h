#pragma once

#include "Grabber.h"
#include <actions/Action.h>
#include <Robot.h>

namespace xero {
    namespace phoenix {
        class Grabber ;

        class GrabberAction : public xero::base::GenericAction {
        public:
            GrabberAction(Grabber &grabber) : GenericAction(grabber.getRobot().getMessageLogger()), grabber_(grabber) {
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
