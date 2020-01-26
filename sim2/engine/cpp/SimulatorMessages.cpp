#include <SimulatorMessages.h>
#include <SimulatorMessageSink.h>
#include <cassert>
#include <iostream>

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
            assert(mt != MessageType::Debug) ;

            std::thread::id id = std::this_thread::get_id() ;
            auto it = per_thread_data_.find(id) ;
            if (it != per_thread_data_.end())
            {
                assert(it->second.msg_.length() == 0);
            }

            MessageData data ;
            data.level_ = 0 ;
            data.mt_ = mt ;
            per_thread_data_.insert_or_assign(id, data) ;
        }

        void SimulatorMessages::startMessage(SimulatorMessages::MessageType mt, int level)
        {
            assert(mt == MessageType::Debug) ;

            std::thread::id id = std::this_thread::get_id() ;
            auto it = per_thread_data_.find(id) ;
            if (it != per_thread_data_.end())
            {
                assert(it->second.msg_.length() == 0);
            }

            MessageData data ;
            data.level_ = level ;
            data.mt_ = mt ;
            per_thread_data_.insert_or_assign(id, data) ;
        }        

        SimulatorMessages &SimulatorMessages::operator<<(const std::string &msg)
        {
            std::thread::id id = std::this_thread::get_id() ;
            auto it = per_thread_data_.find(id) ;
            assert(it != per_thread_data_.end()) ;
            per_thread_data_[id].msg_ += msg ;
            return *this;
        }

        SimulatorMessages &SimulatorMessages::operator<<(int value)
        {
            std::thread::id id = std::this_thread::get_id() ;
            auto it = per_thread_data_.find(id) ;
            assert(it != per_thread_data_.end()) ;
            per_thread_data_[id].msg_ += std::to_string(value);
            return *this;
        }

        SimulatorMessages &SimulatorMessages::operator<<(double value)
        {
            std::thread::id id = std::this_thread::get_id() ;
            auto it = per_thread_data_.find(id) ;
            assert(it != per_thread_data_.end()) ;
            per_thread_data_[id].msg_ += std::to_string(value);
            return *this;
        }            

        SimulatorMessages &SimulatorMessages::operator<<(uint64_t value)
        {
            std::thread::id id = std::this_thread::get_id() ;
            auto it = per_thread_data_.find(id) ;
            assert(it != per_thread_data_.end()) ;
            per_thread_data_[id].msg_ += std::to_string(value);
            return *this;
        }   

        SimulatorMessages &SimulatorMessages::operator<<(unsigned int value)
        {
            std::thread::id id = std::this_thread::get_id() ;
            auto it = per_thread_data_.find(id) ;
            assert(it != per_thread_data_.end()) ;
            per_thread_data_[id].msg_ += std::to_string(value);
            return *this;
        }                        

        void SimulatorMessages::endMessage(int64_t t)
        {
            std::thread::id id = std::this_thread::get_id() ;
            auto it = per_thread_data_.find(id) ;
            assert(it != per_thread_data_.end()) ;

            std::string outmsg = std::to_string(static_cast<double>(t/1e6));

            switch(it->second.mt_)
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

            outmsg += it->second.msg_ ;
            if (it->second.mt_ != MessageType::Debug || (it->second.level_ <= debug_level_))
            {
                for(auto sink : sinks_)
                    sink->output(outmsg);
            }

            it->second.msg_.clear() ;
        }
    }
}
