#pragma once

#include <AutoController.h>
#include <AutoMode.h>
#include <string>

namespace xero {
    namespace grond {
        class GrondAutoModeController : public xero::base::AutoController {
        public:
            GrondAutoModeController(xero::base::Robot &robot) ;
            virtual ~GrondAutoModeController() ;

            virtual void updateAutoMode(int sel, const std::string &gamedata) ;

        private:
            xero::base::AutoModePtr createCharAutoMode() ;
            xero::base::AutoModePtr createTestOne() ;
            xero::base::AutoModePtr createTestTwo() ;
        } ;
    }
}
