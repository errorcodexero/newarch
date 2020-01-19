#pragma once
#include <engine/json.h>
#include <engine/SimValue.h>
#include <map>


namespace xero
{
    namespace sim2
    {
        class SimulatorEngine;
        class SimulationProperties
        {
        private:
            static constexpr const char *HALModelName = "___HAL___" ;

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