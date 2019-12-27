#include "MessageLogger.h"
#include "MessageLoggerDest.h"
#include <sstream>
#include <iostream>

namespace xero
{
    namespace misc
    {

        MessageLogger::MessageLogger()
        {
            //Initialize maps
            subsystems_enabled_ = 0;
            time_func_ = nullptr ;
        }

        MessageLogger::PerThreadData *MessageLogger::getThreadData()
        {
#ifdef SIMULATOR
            //
            // In the simulator, we get per thread data
            //
            auto it = data_.find(std::this_thread::get_id());
            if (it == data_.end())
            {
                PerThreadData data;
                data_[std::this_thread::get_id()] = data;
                it = data_.find(std::this_thread::get_id());        
            }
            return &it->second;
#else
            //
            // On the real robot, there are no threads and we get the 
            // class specific data.
            //
            return &data_;
#endif
        }

        void MessageLogger::clear() 
        {
            destinations_.clear() ;
        }

        void MessageLogger::enableType(const MessageType &type)
        {
            enabled_modes_.push_back(type);
        }

        void MessageLogger::disableType(const MessageType &type)
        {
            auto it = std::find(enabled_modes_.begin(), enabled_modes_.end(), type);
            if (it != enabled_modes_.end())
                enabled_modes_.erase(it);
        }

        void MessageLogger::enableSubsystem(uint64_t sys)
        {
            subsystems_enabled_ |= sys;
        }

        void MessageLogger::disableSubsystem(uint64_t sys)
        {
            subsystems_enabled_ &= ~sys;
        }

        bool MessageLogger::isMessageTypeEnabled(const MessageType &type)
        {
            auto it = std::find(enabled_modes_.begin(), enabled_modes_.end(), type);
            return it != enabled_modes_.end();
        }

        bool MessageLogger::isSubsystemEnabled(uint64_t subs)
        {
            if (subs == 0)
                return true;

            return (subsystems_enabled_ & subs) != 0ull;
        }

        void MessageLogger::startMessage(const MessageType &type)
        {
            auto *data = getThreadData();
            if (data->in_message_)
            {
                data->current_message_ += "FAILED TO CALL ENDMESSAGE";
                endMessage();
            }
            data->current_type_ = type;
            data->in_message_ = true;
            data->current_subsystem_ = 0;
        }

        void MessageLogger::startMessage(const MessageType &type, uint64_t sub)
        {
            auto *data = getThreadData();    
            if (data->in_message_)
            {
                data->current_message_ += "FAILED TO CALL ENDMESSAGE - ";
                endMessage();
            }

            data->current_type_ = type;
            data->in_message_ = true;
            data->current_subsystem_ = sub;
        }

        void MessageLogger::endMessage()
        {
            auto *data = getThreadData();    
            data->in_message_ = false;
            if (data->current_message_.length() > 0) {
                if (isMessageTypeEnabled(data->current_type_) && isSubsystemEnabled(data->current_subsystem_))
                {
                    double now = std::numeric_limits<double>::infinity() ;
                    std::string msg = data->current_message_ ;
                    if (time_func_ != nullptr) {
                        now = (*time_func_)() ;
                        msg = std::to_string(now) + ": " + msg ;
                    }

                    {
        #ifdef SIMULATOR
                        std::lock_guard<std::mutex> lock(output_lock_);
        #endif
                        for (auto dest_p : destinations_)
                            dest_p->displayMessage(data->current_type_, data->current_subsystem_, msg);
                    }
                }
            }
            data->current_message_ = "";
        }

        MessageLogger &MessageLogger::operator<<(const std::string &value)
        {
            auto *data = getThreadData();    
            if (isMessageTypeEnabled(data->current_type_) && isSubsystemEnabled(data->current_subsystem_))
                data->current_message_.append(value);
            return *this;
        }

        MessageLogger &MessageLogger::operator<<(const char *value_p)
        {
            auto *data = getThreadData();        
            if (isMessageTypeEnabled(data->current_type_) && isSubsystemEnabled(data->current_subsystem_))
                data->current_message_.append(std::string(value_p));
            return *this;
        }

        MessageLogger &MessageLogger::operator<<(int32_t value)
        {
            auto *data = getThreadData();        
            if (isMessageTypeEnabled(data->current_type_) && isSubsystemEnabled(data->current_subsystem_))
                data->current_message_.append(std::to_string(value));
            return *this;
        }


        MessageLogger &MessageLogger::operator<<(int64_t value)
        {
            auto *data = getThreadData();        
            if (isMessageTypeEnabled(data->current_type_) && isSubsystemEnabled(data->current_subsystem_))
                data->current_message_.append(std::to_string(value));
            return *this;
        }


        MessageLogger &MessageLogger::operator<<(uint32_t value)
        {
            auto *data = getThreadData();        
            if (isMessageTypeEnabled(data->current_type_) && isSubsystemEnabled(data->current_subsystem_))
                data->current_message_.append(std::to_string(value));
            return *this;
        }


        MessageLogger &MessageLogger::operator<<(uint64_t value)
        {
            auto *data = getThreadData();        
            if (isMessageTypeEnabled(data->current_type_) && isSubsystemEnabled(data->current_subsystem_))
                data->current_message_.append(std::to_string(value));
            return *this;
        }


        MessageLogger &MessageLogger::operator<<(double value)
        {
            auto *data = getThreadData();        
            if (isMessageTypeEnabled(data->current_type_) && isSubsystemEnabled(data->current_subsystem_))
                data->current_message_.append(std::to_string(value));
            return *this;
        }

    } // namespace misc
} // namespace xero
