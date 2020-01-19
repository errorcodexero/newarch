#pragma once

#include <engine/SimulationModel.h>

namespace xero
{
    namespace models
    {
        class WestCoastDrive : public xero::sim2::SimulationModel
        {
        public:
            WestCoastDrive();
            virtual ~WestCoastDrive();

            virtual void run(xero::sim2::SimulatorEngine &engine);

        private:
        };
    }
}