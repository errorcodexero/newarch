#pragma once

#include <AutoController.h>
#include <ActionSequence.h>
#include <string>

namespace xero {
    namespace phoenix {
        class PhoenixAutoController : public xero::base::AutoController {
        public:
            PhoenixAutoController(xero::base::Robot &robot) ;
            virtual ~PhoenixAutoController() ;

			virtual void updateAutoMode(int sel, const std::string &gamedata) ;
        } ;
    }
}
