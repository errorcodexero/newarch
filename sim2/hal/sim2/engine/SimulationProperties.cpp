#include <engine/SimulationProperties.h>
#include <engine/json.h>
#include <fstream>

namespace xero
{
    namespace sim2
    {
        SimulationProperties::SimulationProperties()
        {            
        }

        SimulationProperties::~SimulationProperties()
        {

        }

        bool SimulationProperties::loadProperties(const std::string &path)
        {
            std::ifstream strm(path) ;

            if (!strm.is_open())
                return false ;

            nlohmann::json::json j ;
            j << strm ;
        }

        bool SimulationProperties::hasModelProperty(const std::string &model, const std::string &name)
        {
            std::string fullname = model + ":" + name ;
            auto it = values_.find(fullname) ;
            return it != values_.end() ;
        }

        const SimValue &SimulationProperties::getModelProperty(const std::string &model, const std::string &name)
        {
            std::string fullname = model + ":" + name ;
            auto it = values_.find(fullname) ;

            assert(it != values_.end()) ;
            return it->second ;            
        }
    }
}