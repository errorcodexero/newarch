#pragma once
#include <json.h>
#include <SimValue.h>
#include <map>


namespace xero
{
    namespace sim2
    {
        class SimulatorEngine;
        class SimulationProperties
        {
        public:
            SimulationProperties(SimulatorEngine &engine) ;
            virtual ~SimulationProperties() ;

            bool loadProperties(const std::string &path) ;


        private:
            bool loadModels(nlohmann::json obj);

        private:
            SimulatorEngine &engine_;
        } ;
    }
}