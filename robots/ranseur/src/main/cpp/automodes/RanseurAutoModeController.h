#pragma once

#include <AutoController.h>
#include <AutoMode.h>
#include <string>

namespace xero {
    namespace ranseur {
        class RanseurAutoModeController : public xero::base::AutoController {
        public:
            RanseurAutoModeController(xero::base::Robot &robot) ;
            virtual ~RanseurAutoModeController() ;

            virtual void updateAutoMode(int sel, const std::string &gamedata) ;

        private:
            xero::base::AutoModePtr createTestOne() ;
            xero::base::AutoModePtr testLineFollower() ;
   
            xero::base::AutoModePtr testVision() ;   

        } ;
    }
}
