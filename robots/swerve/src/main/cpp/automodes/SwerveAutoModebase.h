#pragma once

#include <AutoMode.h>

namespace xero {
    namespace swerve {
        class SwerveAutoModeBase : public xero::base::AutoMode
        {
        public:
            SwerveAutoModeBase(xero::base::Robot &robot, const std::string &name, const std::string &desc) ;
            virtual ~SwerveAutoModeBase() ;


        protected:

        private:
        } ;
    }
}
