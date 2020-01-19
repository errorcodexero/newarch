#include <engine/SimValue.h>
#include <engine/EventsManager.h>
#include <engine/SimulationModelEvent.h>
#include <engine/SimulatorEngine.h>
#include <engine/json.h>
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

        bool EventsManager::loadDurations(nlohmann::json obj)
        {
            if (!obj.is_object())
                return false;

            if (!obj.at("start").is_number())
                return false;
            start_ = obj.value("start", 0.0);

            if (!obj.at("auto").is_number())
                return false;
            auto_ = obj.value("auto", 0.0);

            if (!obj.at("teleop").is_number())
                return false;
            teleop_ = obj.value("teleop", 0.0);

            if (!obj.at("test").is_number())
                return false;
            test_ = obj.value("test", 0.0);

            return true;
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
            }
            return true;
        }             

        bool EventsManager::loadEvents(const std::string &path)
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

            msg.startMessage(SimulatorMessages::MessageType::Debug);
            msg << "reading simulation event file '" << path << "'";
            msg.endMessage(engine_.getSimulationTime());                

            nlohmann::json obj = nlohmann::json::parse(strm);
            strm.close();

            if (!loadDurations(obj.at("durations")))
                return false;

            if (!loadStimulus(obj.at("stimulus")))
                return false;

            for(auto ev : events_)
            {
                msg.startMessage(SimulatorMessages::MessageType::Debug);
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