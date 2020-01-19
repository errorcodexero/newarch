#pragma once

#include <string>

namespace xero
{
    namespace sim2
    {
        class SimulatorMessageSink
        {
        public:
            SimulatorMessageSink();
            virtual ~SimulatorMessageSink();

            virtual void output(const std::string &str) = 0 ;

        private:
        };
    }
}