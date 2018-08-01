#pragma once

#include "GrabberAction.h" 

namespace xero {
    namespace phoenix {
         class GrabberToAngleAction : public xero::base::GrabberAction {
            public:
            GrabberToAngleAction(Grabber &grabber);
            virtual ~GrabberToAngleAction(); 
        }
    }
}