#pragma once

#include <AutoController.h>
#include <AutoMode.h>
#include <string>

namespace xero {
    namespace motorcontroller {
        class MotorControllerAutoModeController : public xero::base::AutoController {
        public:
            MotorControllerAutoModeController(xero::base::Robot &robot) ;
            virtual ~MotorControllerAutoModeController() ;

            virtual void updateAutoMode(int sel, const std::string &gamedata) ;

        private:
        } ;
    }
}
