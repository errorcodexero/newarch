#pragma once

#include <AutoController.h>
#include <AutoMode.h>
#include <string>

namespace xero {
    namespace droid {
        class droidAutoModeController : public xero::base::AutoController {
        public:
            droidAutoModeController(xero::base::Robot &robot) ;
            virtual ~droidAutoModeController() ;

            virtual void updateAutoMode(int sel, const std::string &gamedata) ;

        private:
        } ;
    }
}
