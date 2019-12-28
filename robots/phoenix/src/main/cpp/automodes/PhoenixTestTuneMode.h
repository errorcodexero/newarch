#pragma once

#include "PhoenixAutoMode.h"

namespace xero
{
    namespace phoenix
    {
        class PhoenixTestTuneMode : public PhoenixAutoMode
        {
        public:
            PhoenixTestTuneMode(xero::base::Robot &robot);
            virtual ~PhoenixTestTuneMode();

        private:
        };
    }
}