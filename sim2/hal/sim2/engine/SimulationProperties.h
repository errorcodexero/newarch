#pragma once
#include <engine/json.h>
#include <engine/SimValue.h>
#include <map>


namespace xero
{
    namespace sim2
    {
        class SimulationProperties
        {
        private:
            static constexpr const char *HALModelName = "___HAL___" ;

        public:
            SimulationProperties() ;
            virtual ~SimulationProperties() ;

            bool loadProperties(const std::string &path) ;

            bool hasModelProperty(const std::string &inst, const std::string &name) ;
            const SimValue &getModelProperty(const std::string &inst, const std::string &name) ;

            bool hasHALProperty(const std::string &name) {
                return hasModelProperty(HALModelName, name) ;
            }

            const SimValue &getHALProperty(const std::string &name) {
                return getModelProperty(HALModelName, name) ;
            }

            void setValue(const std::string &name, const SimValue &v) {
                values_[name] = v;
            }

        private:
            bool loadModels(nlohmann::json obj);
            bool loadHAL(nlohmann::json obj);

        private:
            std::map<std::string, SimValue> values_ ;
        } ;
    }
}