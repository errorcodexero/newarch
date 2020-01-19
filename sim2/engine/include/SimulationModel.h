#pragma once

#include <SimValue.h>
#include <string>
#include <map>

namespace xero
{
    namespace sim2
    {
        class SimulatorEngine ;
        
        class SimulationModel
        {
        public:
            SimulationModel(const std::string &name);
            virtual ~SimulationModel();

            virtual void run(SimulatorEngine &engine) = 0;

            void setProperty(const std::string &name, const SimValue &value) {
                props_[name] = value;
            }

        private:
            std::map<std::string, SimValue> props_;
            std::string name_;
        };
    }
}
