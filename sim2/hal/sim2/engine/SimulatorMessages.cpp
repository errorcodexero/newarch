#include <engine/SimulatorMessages.h>
#include <engine/SimulatorMessageSink.h>
#include <cassert>

namespace xero
{
    namespace sim2
    {
        SimulatorMessages::SimulatorMessages()
        {
        }

        SimulatorMessages::~SimulatorMessages()
        {
        }

        void SimulatorMessages::startMessage(SimulatorMessages::MessageType mt)
        {
            assert(msg_.length() == 0);
            msg_.clear();
            mt_ = mt;
        }

        SimulatorMessages &SimulatorMessages::operator<<(const std::string &msg)
        {
            msg_ += msg;
            return *this;
        }

        SimulatorMessages &SimulatorMessages::operator<<(int value)
        {
            msg_ += std::to_string(value);
            return *this;
        }

        SimulatorMessages &SimulatorMessages::operator<<(double value)
        {
            msg_ += std::to_string(value);
            return *this;
        }                

        void SimulatorMessages::endMessage(int64_t t)
        {
            std::string outmsg = std::to_string(t);

            switch(mt_)
            {
            case MessageType::Error:
                outmsg += ": error: ";
                break;

            case MessageType::Warning:
                outmsg += ": warning : ";
                break;

            case MessageType::Debug:
                outmsg += ": debug : ";
                break;  

            case MessageType::Info:
                outmsg += ": info : ";
                break;                                  
            }

            outmsg += msg_;
            msg_.clear();

            for(auto sink : sinks_)
                sink->output(outmsg);
        }
    }
}
