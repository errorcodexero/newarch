#pragma once

namespace xero
{
    namespace sim2
    {
        class SimulatorEngine ;
        
        class SimulationModel
        {
        public:
            SimulationModel();
            virtual ~SimulationModel();

            virtual void run(SimulatorEngine &engine) = 0;

        private:
        };
    }
}
