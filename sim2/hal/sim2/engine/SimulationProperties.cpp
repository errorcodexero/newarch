#include <engine/SimulationProperties.h>
#include <fstream>
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

        bool SimulationProperties::loadModels(nlohmann::json obj)
        {
            std::string model;
            std::string inst;

            if (!obj.is_array())
                return false;

            for (size_t i = 0; i < obj.size(); i++)
            {
                nlohmann::json elem = obj[i];
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
                        bool bv = value.value(key, false);
                        SimValue sv(bv);
                        setValue(key, sv);
                    }
                    else if (value.is_number())
                    {
                        double ipart;
                        double v = value.value(key, 0.0);

                        if (std::modf(v, &ipart) < 1e-9)
                        {
                            SimValue sv(static_cast<int>(ipart));
                            setValue(key, sv);
                        }
                        else
                        {
                            SimValue sv(v);
                            setValue(key, sv);
                        }
                    }
                    else if (value.is_string())
                    {
                        SimValue sv(value.value(key, ""));
                        setValue(key, sv);
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
                        bool bv = value.value(key, false);
                        SimValue sv(bv);
                        setValue(key, sv);
                    }
                    else if (value.is_number())
                    {
                        double ipart;
                        double v = value.value(key, 0.0);

                        if (std::modf(v, &ipart) < 1e-9)
                        {
                            SimValue sv(static_cast<int>(ipart));
                            setValue(key, sv);
                        }
                        else
                        {
                            SimValue sv(v);
                            setValue(key, sv);
                        }
                    }
                    else if (value.is_string())
                    {
                        SimValue sv(value.value(key, ""));
                        setValue(key, sv);
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

            nlohmann::json model = obj.at("models");

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