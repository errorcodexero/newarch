#pragma once

#include <AutoController.h>
#include <AutoMode.h>
#include <string>

namespace xero {
    namespace $$robotname$$ {
        class $$RobotName$$AutoModeController : public xero::base::AutoController {
        public:
            $$RobotName$$AutoModeController(xero::base::Robot &robot) ;
            virtual ~$$RobotName$$AutoModeController() ;

            virtual void updateAutoMode(int sel, const std::string &gamedata) ;

        private:
        } ;
    }
}
