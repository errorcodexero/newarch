#pragma once

#include <json.h>
#include <list>
#include <memory>
#include <string>

namespace xero
{
    namespace sim2
    {
        class SimulationEvent ;
        class SimulatorEngine;

        class EventsManager
        {
        public:
            EventsManager(SimulatorEngine &engine) ;
            virtual ~EventsManager() ;

            bool loadEvents(const std::string &simfile) ;
            void addEvent(double t, const std::string &model, const std::string &inst, const std::string &name, const SimValue &value);

            size_t size() {
                return events_.size() ;
            }

            std::shared_ptr<SimulationEvent> front() {
                return events_.front() ;
            }

            void pop_front() {
                events_.pop_front() ;
            }

        private:
            bool loadStimulus(nlohmann::json obj);
            void insertEvent(std::shared_ptr<SimulationEvent> ev);

        private:
            std::list<std::shared_ptr<SimulationEvent>> events_;
            SimulatorEngine &engine_;
        } ;
    }
}