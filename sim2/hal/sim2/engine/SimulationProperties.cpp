#include <engine/SimulationProperties.h>
#include <engine/SimulatorEngine.h>
#include <engine/SimulationModel.h>
#include <iostream>
#include <fstream>
#include <cmath>

namespace xero
{
    namespace sim2
    {
        SimulationProperties::SimulationProperties(SimulatorEngine &engine) : engine_(engine)
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

                std::shared_ptr<SimulationModel> siminst = engine_.createModelInstance(model, inst);
                if (siminst == nullptr)
                    return false;

                obj = elem["hardware"];
                for(const auto &elem : obj.items())
                {
                    auto key = elem.key();
                    auto value = elem.value();

                    std::string fullkey = "hw:" + key;

                    if (value.is_boolean())
                    {
                        SimValue sv(obj.value(key, false));
                        siminst->setProperty(fullkey, sv);
                    }
                    else if (value.is_number_integer())
                    {
                        SimValue sv(obj.value(key, static_cast<int>(0)));
                        siminst->setProperty(fullkey, sv);                     
                    }
                    else if (value.is_number_float())
                    {
                        SimValue sv(obj.value(key, static_cast<double>(0.0)));
                        siminst->setProperty(fullkey, sv);
                    }
                    else if (value.is_string())
                    {
                        SimValue sv(obj.value(key, ""));
                        siminst->setProperty(fullkey, sv);
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

                    std::string fullkey = key;

                    if (value.is_boolean())
                    {
                        SimValue sv(obj.value(key, false));
                        siminst->setProperty(fullkey, sv);
                    }
                    else if (value.is_number_integer())
                    {
                        SimValue sv(obj.value(key, static_cast<int>(0)));
                        siminst->setProperty(fullkey, sv);                      
                    }
                    else if (value.is_number_float())
                    {
                        SimValue sv(obj.value(key, static_cast<double>(0.0)));
                        siminst->setProperty(fullkey, sv);
                    }
                    else if (value.is_string())
                    {
                        SimValue sv(obj.value(key, ""));
                        siminst->setProperty(fullkey, sv);
                    }
                    else
                    {
                        return false;
                    }
                }                
            }

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

            return true;
        }
    }
}