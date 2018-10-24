#pragma once

#include "ControllerBase.h"

namespace xero {
    namespace base {
        class TeleopController : public ControllerBase {
        public:
            TeleopController(Robot &robot) ;
            virtual ~TeleopController() ;

            virtual void run() ;

        private:
        } ;
    }
}