#pragma once

#include <AutoMode.h>

namespace xero {
    namespace ranseur {
        class RanseurAutoModeBase : public xero::base::AutoMode
        {
        public:
            RanseurAutoModeBase(xero::base::Robot &robot, const std::string &name, const std::string &desc) ;
            virtual ~RanseurAutoModeBase() ;


        protected:
      
        private:
        } ;
    }
}