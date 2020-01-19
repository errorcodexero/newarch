#include <SimulationProperties.h>
#include <SimulatorEngine.h>
#include <SimulationModel.h>
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
            auto &msg = engine_.getMessageOutput();

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
                {
                    msg.startMessage(SimulatorMessages::MessageType::Error);
                    msg << "could not create model instance '" << inst << "' of type '" << model << "' - no such model type exists";
                    msg.endMessage(engine_.getSimulationTime());  
                    return false;
                }

                msg.startMessage(SimulatorMessages::MessageType::Debug, 2);
                msg << "created model instance '" << inst << "' of type '" << model << "'";
                msg.endMessage(engine_.getSimulationTime()); 

                obj = elem["hardware"];
                for(const auto &elem : obj.items())
                {
                    auto key = elem.key();
                    auto value = elem.value();
                    SimValue sv;

                    std::string fullkey = "hw:" + key;

                    if (value.is_boolean())
                    {
                        sv = SimValue(obj.value(key, false));

                    }
                    else if (value.is_number_integer())
                    {
                        sv = SimValue(obj.value(key, static_cast<int>(0)));
                    }
                    else if (value.is_number_float())
                    {
                        sv = SimValue(obj.value(key, static_cast<double>(0.0)));
                    }
                    else if (value.is_string())
                    {
                        sv = SimValue(obj.value(key, ""));
                    }
                    else
                    {
                        msg.startMessage(SimulatorMessages::MessageType::Error);
                        msg << "invalid property '" << key << "' - on model instance '" << inst << "'";
                        msg.endMessage(engine_.getSimulationTime());                         
                        return false;
                    }

                    msg.startMessage(SimulatorMessages::MessageType::Debug, 5);
                    msg << "  added hw property '" << key << "' with value '" << sv.toString();
                    msg.endMessage(engine_.getSimulationTime()); 

                    siminst->setProperty(fullkey, sv);                    
                }

                obj = elem["behavior"];
                for(const auto &elem : obj.items())
                {
                    auto key = elem.key();
                    auto value = elem.value();
                    SimValue sv;

                    std::string fullkey = key;

                    if (value.is_boolean())
                    {
                        sv = SimValue(obj.value(key, false));
                    }
                    else if (value.is_number_integer())
                    {
                        sv = SimValue(obj.value(key, static_cast<int>(0)));
                    }
                    else if (value.is_number_float())
                    {
                        sv = SimValue(obj.value(key, static_cast<double>(0.0)));
                    }
                    else if (value.is_string())
                    {
                        sv = SimValue(obj.value(key, ""));
                    }
                    else
                    {
                        msg.startMessage(SimulatorMessages::MessageType::Error);
                        msg << "invalid property '" << key << "' - on model instance '" << inst << "'";
                        msg.endMessage(engine_.getSimulationTime());                         
                        return false;
                    }

                    msg.startMessage(SimulatorMessages::MessageType::Debug, 5);
                    msg << "  added model property '" << key << "' with value '" << sv.toString();
                    msg.endMessage(engine_.getSimulationTime()); 

                    siminst->setProperty(fullkey, sv); 
                }                
            }

            return true;
        }

        bool SimulationProperties::loadProperties(const std::string &path)
        {
            auto &msg = engine_.getMessageOutput();

            std::ifstream strm(path) ;

            if (!strm.is_open())
            {
                msg.startMessage(SimulatorMessages::MessageType::Error);
                msg << "could not open robot properties file '" << path << "'";
                msg.endMessage(engine_.getSimulationTime());  

                return false ;
            }

            msg.startMessage(SimulatorMessages::MessageType::Debug, 1);
            msg << "reading robot properties file '" << path << "'";
            msg.endMessage(engine_.getSimulationTime());

            nlohmann::json obj = nlohmann::json::parse(strm);
            strm.close();

            if (!loadModels(obj.at("models")))
                return false;

            return true;
        }
    }
}