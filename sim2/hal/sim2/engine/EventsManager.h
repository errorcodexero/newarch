#pragma once

#include <engine/json.h>
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

            double getStartDuration() const {
                return start_;
            }

            double getAutoDuration() const {
                return auto_;
            }

            double getTeleopDuration() const {
                return teleop_;
            }

            double getTestDuration() const {
                return test_;
            }

            bool loadEvents(const std::string &simfile) ;

            void addEvent(double t, const std::string &model, const std::string &inst, const std::string &name, const SimValue &value);

        private:
            bool loadDurations(nlohmann::json obj);
            bool loadStimulus(nlohmann::json obj);

            void insertEvent(std::shared_ptr<SimulationEvent> ev);

        private:
            double start_;
            double auto_;
            double teleop_;
            double test_;

            std::list<std::shared_ptr<SimulationEvent>> events_;

            SimulatorEngine &engine_;
        } ;
    }
}