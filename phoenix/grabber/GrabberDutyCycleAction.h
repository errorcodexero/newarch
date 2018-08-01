#pragma once

#include "GrabberAction.h" 

namespace xero {
    namespace phoenix {
         class GrabberDutyCycleAction : public xero::base::GrabberAction {
            public:
            GrabberDutyCycleAction(Grabber &grabber, double v);
            virtual ~GrabberDutyCycleAction(); 
        
        }
    }
}