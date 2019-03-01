#pragma once

#include <AutoMode.h>

namespace xero {
    namespace phaser {
        class PhaserAutoModeBase : public xero::base::AutoMode
        {
        public:
            PhaserAutoModeBase(xero::base::Robot &robot, const std::string &name, const std::string &desc) ;
            virtual ~PhaserAutoModeBase() ;

        private:
        } ;
    }
}