#pragma once

#include <engine/SimValue.h>
#include <string>

namespace xero
{
    namespace sim2
    {
        class SimulationEvent
        {
        public:

        public:
            SimulationEvent(double t) {
                time_ = t;
            }

            virtual ~SimulationEvent() {
            }

            double time() const {
                return time_;
            }

            virtual std::string toString() const = 0;

        private:
            double time_;

        };
    }
}