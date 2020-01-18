#include <engine/SimulationProperties.h>
#include <fstream>
#include <iostream>
#include <cmath>

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

        bool SimulationProperties::loadModels(nlohmann::json mobj)
        {
            std::string model;
            std::string inst;

            if (!mobj.is_array())
                return false;

            for (size_t i = 0; i < mobj.size(); i++)
            {
                nlohmann::json elem = mobj[i];
                nlohmann::json obj;

                if (!elem.is_object())
                    return false;

                if (!elem["model"].is_string())
                    return false;

                if (!elem["instance"].is_string())
                    return false;                    

                if (!elem["hardware"].is_object())
                    return false;

                if (!elem["behavior"].is_object())
                    return false;                    

                model = elem.value("model", "");
                if (model.length() == 0)
                    return false;

                inst = elem.value("instance", "");
                if (inst.length() == 0)
                    return false;

                obj = elem["hardware"];
                for(const auto &elem : obj.items())
                {
                    auto key = elem.key();
                    auto value = elem.value();

                    std::string fullkey = "hw:" + model + ":" + inst + ":" + key;

                    if (value.is_boolean())
                    {
                        SimValue sv(obj.value(key, false));
                        setValue(fullkey, sv);
                    }
                    else if (value.is_number_integer())
                    {
                        SimValue sv(obj.value(key, static_cast<int>(0)));
                        setValue(fullkey, sv);                        
                    }
                    else if (value.is_number_float())
                    {
                        SimValue sv(obj.value(key, static_cast<double>(0.0)));
                        setValue(fullkey, sv);
                    }
                    else if (value.is_string())
                    {
                        SimValue sv(obj.value(key, ""));
                        setValue(fullkey, sv);
                    }
                    else
                    {
                        return false;
                    }
                }

                obj = elem["behavior"];
                for(const auto &elem : obj.items())
                {
                    auto key = elem.key();
                    auto value = elem.value();

                    std::string fullkey = model + ":" + inst + ":" + key;

                    if (value.is_boolean())
                    {
                        SimValue sv(obj.value(key, false));
                        setValue(fullkey, sv);
                    }
                    else if (value.is_number_integer())
                    {
                        SimValue sv(obj.value(key, static_cast<int>(0)));
                        setValue(fullkey, sv);                        
                    }
                    else if (value.is_number_float())
                    {
                        SimValue sv(obj.value(key, static_cast<double>(0.0)));
                        setValue(fullkey, sv);
                    }
                    else if (value.is_string())
                    {
                        SimValue sv(obj.value(key, ""));
                        setValue(fullkey, sv);
                    }
                    else
                    {
                        return false;
                    }
                }                
            }

            return true;
        }

        bool SimulationProperties::loadHAL(nlohmann::json obj)
        {
            return true;
        }

        bool SimulationProperties::loadProperties(const std::string &path)
        {
            std::ifstream strm(path) ;

            if (!strm.is_open())
                return false ;

            nlohmann::json obj = nlohmann::json::parse(strm);
            strm.close();

            if (!loadModels(obj.at("models")))
                return false;

            if (!loadHAL(obj.at("hal")))
                return false;

            return true;
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