#pragma once

#include "Action.h" 

namespace xero {
    namespace phoenix {
        class Grabber;
        class GrabberAction : public xero::base::Action {
            public:
                GrabberAction(Grabber &grabber);
                virtual ~GrabberAction(); 
            protected:
                Grabber & getGrabber() {
                    return grabber_ ;
                }
            private:
                Grabber &grabber_ ;
        }
    }
}