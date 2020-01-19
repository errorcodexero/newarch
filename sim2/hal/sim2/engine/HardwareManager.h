#pragma once
namespace xero
{
    namespace sim2
    {
        class SimulatorEngine ;

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
        }
    }
}