#pragma once

#include "GrabberAction.h" 

namespace xero {
    namespace phoenix {
         class GrabberDutyCycleAction : public xero::phoenix::GrabberAction {
            public:
            GrabberDutyCycleAction(Grabber &grabber, double v);
            virtual ~GrabberDutyCycleAction(); 
        
        };
    }
}