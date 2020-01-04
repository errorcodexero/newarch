#pragma once

#include <AutoMode.h>

namespace xero {
    namespace droid {
        class droidAutoModeBase : public xero::base::AutoMode
        {
        public:
            droidAutoModeBase(xero::base::Robot &robot, const std::string &name, const std::string &desc) ;
            virtual ~droidAutoModeBase() ;


        protected:

        private:
        } ;
    }
}
