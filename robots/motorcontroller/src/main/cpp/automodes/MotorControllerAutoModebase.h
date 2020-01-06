#pragma once

#include <AutoMode.h>

namespace xero {
    namespace motorcontroller {
        class MotorControllerAutoModeBase : public xero::base::AutoMode
        {
        public:
            MotorControllerAutoModeBase(xero::base::Robot &robot, const std::string &name, const std::string &desc) ;
            virtual ~MotorControllerAutoModeBase() ;


        protected:

        private:
        } ;
    }
}
