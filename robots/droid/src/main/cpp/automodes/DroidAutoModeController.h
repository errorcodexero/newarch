#pragma once

#include <AutoController.h>
#include <AutoMode.h>
#include <string>

namespace xero {
    namespace droid {
        class DroidAutoModeController : public xero::base::AutoController {
        public:
            DroidAutoModeController(xero::base::Robot &robot) ;
            virtual ~DroidAutoModeController() ;

            virtual void updateAutoMode(int sel, const std::string &gamedata) ;

        private:
            bool testmode_;
        } ;
    }
}
