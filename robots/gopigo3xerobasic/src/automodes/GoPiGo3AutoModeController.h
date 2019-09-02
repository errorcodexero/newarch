#pragma once

#include <AutoController.h>
#include <memory>

namespace xero {
    namespace gopigo3 {
        class GoPiGo3AutoModeController : public xero::base::AutoController {
        public:
            GoPiGo3AutoModeController(xero::base::Robot &robot) ;
            virtual ~GoPiGo3AutoModeController() ;

            virtual void updateAutoMode(int sel, const std::string &gamedata) ;
        } ;
    }
}
