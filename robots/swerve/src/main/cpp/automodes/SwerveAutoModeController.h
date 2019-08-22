#pragma once

#include <AutoController.h>
#include <AutoMode.h>
#include <string>

namespace xero {
    namespace swerve {
        class SwerveAutoModeController : public xero::base::AutoController {
        public:
            SwerveAutoModeController(xero::base::Robot &robot) ;
            virtual ~SwerveAutoModeController() ;

            virtual void updateAutoMode(int sel, const std::string &gamedata) ;

        private:
            xero::base::AutoModePtr testSwerveModule() ;     
        } ;
    }
}
