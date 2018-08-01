#pragma once

#include "GrabberAction.h" 

namespace xero {
    namespace phoenix {
         class GrabberHoldCubeAction : public xero::base::GrabberAction {
            public:
            GrabberHoldCubeAction(Grabber &grabber);
            virtual ~GrabberHoldCubeAction(); 
        }
    }
}