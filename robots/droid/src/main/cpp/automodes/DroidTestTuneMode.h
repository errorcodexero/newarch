#pragma once

#include "DroidAutoMode.h"

namespace xero
{
    namespace droid
    {
        class DroidTestTuneMode : public DroidAutoMode
        {
        public:
            DroidTestTuneMode(xero::base::Robot &robot);
            virtual ~DroidTestTuneMode();

        private:
        };
    }
}