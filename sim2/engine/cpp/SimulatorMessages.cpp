#include <SimulatorMessages.h>
#include <SimulatorMessageSink.h>
#include <cassert>

namespace xero
{
    namespace sim2
    {
        SimulatorMessages::SimulatorMessages()
        {
            // Everything less than or equal to 10 is going to print
            debug_level_ = 10;
        }

        SimulatorMessages::~SimulatorMessages()
        {
        }

        void SimulatorMessages::startMessage(SimulatorMessages::MessageType mt)
        {
            assert(msg_.length() == 0);
            assert(mt != MessageType::Debug);

            msg_.clear();
            mt_ = mt;
        }

        void SimulatorMessages::startMessage(SimulatorMessages::MessageType mt, int level)
        {
            assert(msg_.length() == 0);
            assert(mt == MessageType::Debug);
            
            msg_.clear();
            mt_ = mt;
            level_ = level;
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

            if (mt_ != MessageType::Debug || (level_ <= debug_level_))
            {
                for(auto sink : sinks_)
                    sink->output(outmsg);
            }
        }
    }
}
