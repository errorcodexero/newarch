#pragma once

#include <string>

namespace xero
{
    namespace sim2
    {
        class SimulationEvent
        {
        public:
            enum class EventType
            {
                ModelEvent,
                HALEvent,
            };

        public:
            SimulationEvent(const std::string &model);

        private:
            EventType type_;
            std::string target_;
        };
    }
}