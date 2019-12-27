#pragma once

#include <AutoMode.h>

namespace xero
{
    namespace phoenix
    {
        class PhoenixAutoMode : public xero::base::AutoMode
        {
        public:
            PhoenixAutoMode(xero::base::Robot &robot, const std::string &name, const std::string &desc);
            virtual ~PhoenixAutoMode();

        private:
        };
    }
}