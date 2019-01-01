#pragma once

#include <AutoController.h>
#include <memory>

namespace xero {
    namespace gopigo {
        class GoPiGo3AutoModeController : public xero::base::AutoController {
        public:
            GoPiGo3AutoModeController(xero::base::Robot &robot) ;
            virtual ~GoPiGo3AutoModeController() ;

            virtual void updateAutoMode(int sel, const std::string &gamedata) ;

        private:
            xero::base::ActionSequencePtr createRotatePos90() ;
            xero::base::ActionSequencePtr createRotatePos45() ;         
            xero::base::ActionSequencePtr createRotatePos135() ;            
            xero::base::ActionSequencePtr createRotateNeg90() ;
            xero::base::ActionSequencePtr createServoAutoMode();
            xero::base::ActionSequencePtr createLEDAutoMode();
        } ;
    }
}
