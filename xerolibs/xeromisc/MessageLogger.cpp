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

std::shared_ptr<MessageLogger::InMessageData> MessageLogger::getMessageData()
{
    std::shared_ptr<InMessageData> data ;

#ifdef SIMULATOR
    //
    // In the case of the simulator, get the message data on a per thread
    // basic
    //
    auto it = mdata_.find(std::this_thread::get_id()) ;
    if (it == mdata_.end())
    {
        std::shared_ptr<InMessageData> ind = std::make_shared<InMessageData>() ;
        mdata_[std::this_thread::get_id()] = ind ;
    }

    data = mdata_[std::this_thread::get_id()] ;

#else

    if (mdata_ == nullptr)
        mdata_ = std::make_shared<InMessageData>() ;
    data = mdata_ ;
#endif

    return data ;
}

void MessageLogger::startMessage(const MessageType &type)
{
    auto mdata = getMessageData() ;

    if (mdata->in_message_)
    {
        mdata->current_message_ += "FAILED TO CALL ENDMESSAGE";
        endMessage();
    }
    mdata->current_type_ = type;
    mdata->in_message_ = true;
    mdata->current_subsystem_ = 0;
}

void MessageLogger::startMessage(const MessageType &type, uint64_t sub)
{
    auto mdata = getMessageData() ;

    if (mdata->in_message_)
    {
        mdata->current_message_ += "FAILED TO CALL ENDMESSAGE - ";
        endMessage();
    }

    mdata->current_type_ = type;
    mdata->in_message_ = true;
    mdata->current_subsystem_ = sub;
}

void MessageLogger::endMessage()
{
    auto mdata = getMessageData() ;
        
    mdata->in_message_ = false;
    if (mdata->current_message_.length() > 0) {
        if (isMessageTypeEnabled(mdata->current_type_) && isSubsystemEnabled(mdata->current_subsystem_))
        {
            double now = std::numeric_limits<double>::infinity() ;
            std::string msg = mdata->current_message_ ;
            if (time_func_ != nullptr) {
                now = (*time_func_)() ;

                msg = std::to_string(now) + ": " + msg ;
            }
                

            {
#ifdef SIMULATOR                
                std::lock_guard<std::mutex> destlock(destination_lock_) ;
#endif
                for (auto dest_p : destinations_)
                    dest_p->displayMessage(mdata->current_type_, mdata->current_subsystem_, msg);
            }
        }
    }
    mdata->current_message_ = "";
}

MessageLogger &MessageLogger::operator<<(const std::string &value)
{
    auto mdata = getMessageData() ;

    if (isMessageTypeEnabled(mdata->current_type_) && isSubsystemEnabled(mdata->current_subsystem_))
        mdata->current_message_.append(value);
    return *this;
}

MessageLogger &MessageLogger::operator<<(const char *value_p)
{
    auto mdata = getMessageData() ;

    if (isMessageTypeEnabled(mdata->current_type_) && isSubsystemEnabled(mdata->current_subsystem_))
        mdata->current_message_.append(std::string(value_p));
    return *this;
}

MessageLogger &MessageLogger::operator<<(int32_t value)
{
    auto mdata = getMessageData() ;

    if (isMessageTypeEnabled(mdata->current_type_) && isSubsystemEnabled(mdata->current_subsystem_))
        mdata->current_message_.append(std::to_string(value));
    return *this;
}


MessageLogger &MessageLogger::operator<<(int64_t value)
{
    auto mdata = getMessageData() ;

    if (isMessageTypeEnabled(mdata->current_type_) && isSubsystemEnabled(mdata->current_subsystem_))
        mdata->current_message_.append(std::to_string(value));
    return *this;
}


MessageLogger &MessageLogger::operator<<(uint32_t value)
{
    auto mdata = getMessageData() ;

    if (isMessageTypeEnabled(mdata->current_type_) && isSubsystemEnabled(mdata->current_subsystem_))
        mdata->current_message_.append(std::to_string(value));
    return *this;
}


MessageLogger &MessageLogger::operator<<(uint64_t value)
{
    auto mdata = getMessageData() ;

    if (isMessageTypeEnabled(mdata->current_type_) && isSubsystemEnabled(mdata->current_subsystem_))
        mdata->current_message_.append(std::to_string(value));
    return *this;
}


MessageLogger &MessageLogger::operator<<(double value)
{
    auto mdata = getMessageData() ;

    if (isMessageTypeEnabled(mdata->current_type_) && isSubsystemEnabled(mdata->current_subsystem_))
        mdata->current_message_.append(std::to_string(value));
    return *this;
}

} // namespace misc
} // namespace xero
