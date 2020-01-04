#pragma once

#include <AutoMode.h>

namespace xero
{
    namespace droid
    {
        class DroidAutoMode : public xero::base::AutoMode
        {
        public:
            DroidAutoMode(xero::base::Robot &robot, const std::string &name, const std::string &desc);
            virtual ~DroidAutoMode();

        private:
        };
    }
}