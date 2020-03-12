#include <SimValue.h>
#include <EventsManager.h>
#include <SimulationModelEvent.h>
#include <SimulationAssert.h>
#include <SimulatorEngine.h>
#include <json.h>
#include <fstream>
#include <iostream>

namespace xero
{
    namespace sim2
    {
        EventsManager::EventsManager(SimulatorEngine &engine) : engine_(engine)
        {

        }

        EventsManager::~EventsManager()
        {
            
        }   

        bool EventsManager::loadEvents(double t, nlohmann::json timept)
        {
            nlohmann::json evs = timept.at("events");
            if (!evs.is_array())
                return false;

            // Process all events in this time point
            for (size_t j = 0; j < evs.size(); j++)
            {
                nlohmann::json single = evs[j];

                if (!single.at("model").is_string())
                    return false;

                if (!single.at("instance").is_string())
                    return false;

                std::string model = single.value("model", "");
                std::string inst = single.value("instance", "");

                nlohmann::json values = single.at("values");
                if (!values.is_object())
                    return false;

                for(const auto &item : values.items())
                {
                    SimValue sv;

                    if (item.value().is_string())
                        sv = values.value(item.key(), "");
                    else if (item.value().is_number_integer())
                        sv = values.value(item.key(), static_cast<int>(0));
                    else if (item.value().is_number_float())
                        sv = values.value(item.key(), static_cast<double>(0.0));
                    else if (item.value().is_boolean())
                        sv = values.value(item.key(), false);

                    addEvent(t, model, inst, item.key(), sv);
                }
            }

            return true ;
        }

        bool EventsManager::loadAsserts(double t, nlohmann::json timept)
        {
            nlohmann::json evs = timept.at("asserts");
            if (!evs.is_array())
                return false;

            // Process all events in this time point
            for (size_t j = 0; j < evs.size(); j++)
            {
                nlohmann::json single = evs[j];

                if (!single.at("subsystem").is_string())
                    return false;

                if (!single.at("property").is_string())
                    return false;

                if (!single.at("type").is_string())
                    return false ;

                std::string subsystem = single.value("subsystem", "");
                std::string property = single.value("property", "");                    

                std::string ptype = single.value("type", "") ;
                if (ptype != "integer" && ptype != "double" && ptype != "boolean" && ptype != "string")
                {
                    engine_.getMessageOutput().startMessage(SimulatorMessages::MessageType::Error) ;
                    engine_.getMessageOutput() << "invalid assert type '" << ptype << "', expected 'integer' or 'double' or 'boolean' or 'string'" ;
                    engine_.getMessageOutput().endMessage(engine_.getSimulationTime()) ;
                    return false ;
                }

                if (ptype == "double")
                {
                    if (!single.at("value").is_number_float())
                        return false ;

                    if (!single.at("delta").is_number_float())
                        return false ;                    


                    double value = single.value("value", 0.0) ;
                    double delta = single.value("delta", 0.0) ;
                    auto ev = std::make_shared<SimulationAssert>(t, subsystem, property, value, delta) ;
                    insertEvent(ev) ;                    
                }
                else if (ptype == "integer")
                {
                    if (!single.at("value").is_number_integer())
                        return false ;             

                    int value = single.value("value", 0) ;
                    auto ev = std::make_shared<SimulationAssert>(t, subsystem, property, value) ;
                    insertEvent(ev) ;                    
                }
                else if (ptype == "boolean")
                {
                    if (!single.at("value").is_boolean())
                        return false ;             

                    bool value = single.value("value", false) ;
                    auto ev = std::make_shared<SimulationAssert>(t, subsystem, property, value) ;
                    insertEvent(ev) ;                    
                }  
                else if (ptype == "string")
                {
                    if (!single.at("value").is_string())
                        return false ;             

                    std::string value = single.value("value", "") ;
                    auto ev = std::make_shared<SimulationAssert>(t, subsystem, property, value) ;
                    insertEvent(ev) ;                    
                }                                                   
            }

            return true ;
        }        

        bool EventsManager::loadStimulus(nlohmann::json stobj)
        {
            if (!stobj.is_array())
                return false;

            // Process all time points
            for (size_t i = 0; i < stobj.size() ; i++)
            {
                nlohmann::json timept = stobj[i];
                if (!timept.at("time").is_number())
                    return false;

                double t = timept.value("time", 0.0);

                if (timept.contains("events"))
                {
                    if (!loadEvents(t, timept))
                        return false ;
                }

                if (timept.contains("asserts"))
                {
                    if (!loadAsserts(t, timept))
                        return false ;
                }
            }
            return true;
        }             

        bool EventsManager::loadTimeline(const std::string &path)
        {
            auto &msg = engine_.getMessageOutput();
            std::ifstream strm(path) ;

            if (!strm.is_open())
            {
                msg.startMessage(SimulatorMessages::MessageType::Error);
                msg << "could not open simulation event file '" << path << "'";
                msg.endMessage(engine_.getSimulationTime());  

                return false ;
            }

            msg.startMessage(SimulatorMessages::MessageType::Debug, 0);
            msg << "reading simulation event file '" << path << "'";
            msg.endMessage(engine_.getSimulationTime());                

            nlohmann::json obj ;
            try {
                obj = nlohmann::json::parse(strm);
                strm.close();
            }
            catch(const std::exception &ex)
            {
                msg.startMessage(SimulatorMessages::MessageType::Error) ;
                msg << "error reading simulation event file '" << path << "'" ;
                msg << " - " << ex.what() ;
                msg.endMessage(engine_.getSimulationTime());                 
            }

            if (!loadStimulus(obj.at("stimulus")))
                return false;

            for(auto ev : events_)
            {
                msg.startMessage(SimulatorMessages::MessageType::Debug, 8);
                msg << "  event defined '" << ev->toString() << "'";
                msg.endMessage(engine_.getSimulationTime());
            }

            return true;
        }     

        void EventsManager::addEvent(double t, const std::string &model, const std::string &inst, const std::string &name, const SimValue &value)
        {
            auto ev = std::make_shared<SimulationModelEvent>(t, model, inst, name, value);
            insertEvent(ev);
        }

        void EventsManager::addAssert(double t, const std::string &sub, const std::string &prop, double value, double delta)
        {

        }

        void EventsManager::insertEvent(std::shared_ptr<SimulationEvent> ev)
        {
            if (events_.size() == 0)
            {
                events_.push_back(ev);
            }
            else
            {
                bool inserted = false;

                for (auto it = events_.begin(); it != events_.end() ; it++)
                {
                    if (ev->time() < (*it)->time())
                    {
                        events_.insert(it, ev);
                        inserted = true;
                        break;
                    }
                }

                if (!inserted)
                    events_.push_back(ev);
            }
        }
    }
}