#pragma once

#include "SimulatorMessageSink.h"
#include <string>
#include <iostream>
#include <ostream>

namespace xero
{
    namespace sim2
    {
        class SimulatorStreamMessageSink : public SimulatorMessageSink
        {
        public:
            SimulatorStreamMessageSink(std::ostream &strm) : strm_(strm)
            {
            }

            virtual ~SimulatorStreamMessageSink()
            {                
            }

            void output(const std::string &txt)
            {
                strm_ << txt << std::endl;
            }

        private:
            std::ostream &strm_;
        };
    }
}