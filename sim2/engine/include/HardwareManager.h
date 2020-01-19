#pragma once

#include <SimulatorEngine.h>
#include <SimulatorMessages.h>

namespace xero
{
    namespace sim2
    {
        class HardwareManager
        {
        public:
            HardwareManager(SimulatorEngine &engine) : engine_(engine)
            {

            }

            virtual ~HardwareManager()
            {

            }


        protected:
            SimulatorEngine &getEngine() 
            {
                return engine_;
            }      

        private:
            SimulatorEngine &engine_;
        };
    }
}