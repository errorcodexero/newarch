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
    current_message_ = "";
    in_message_ = false;
    subsystems_enabled_ = 0;
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
    if (in_message_)
    {
        current_message_ += "FAILED TO CALL ENDMESSAGE";
        endMessage();
    }
    current_type_ = type;
    in_message_ = true;
    current_subsystem_ = 0;
}

void MessageLogger::startMessage(const MessageType &type, uint64_t sub)
{
    if (in_message_)
    {
        current_message_ += "FAILED TO CALL ENDMESSAGE - ";
        endMessage();
    }

    current_type_ = type;
    in_message_ = true;
    current_subsystem_ = sub;
}

void MessageLogger::endMessage()
{
    in_message_ = false;
    if (isMessageTypeEnabled(current_type_) && isSubsystemEnabled(current_subsystem_))
    {
        for (auto dest_p : destinations_)
            dest_p->displayMessage(current_type_, current_subsystem_, current_message_);
    }
    current_message_ = "";
}

MessageLogger &MessageLogger::operator<<(const std::string &value)
{
    if (isMessageTypeEnabled(current_type_) && isSubsystemEnabled(current_subsystem_))
        current_message_.append(value);
    return *this;
}

MessageLogger &MessageLogger::operator<<(const char *value_p)
{
    if (isMessageTypeEnabled(current_type_) && isSubsystemEnabled(current_subsystem_))
        current_message_.append(std::string(value_p));
    return *this;
}

MessageLogger &MessageLogger::operator<<(int value)
{
    if (isMessageTypeEnabled(current_type_) && isSubsystemEnabled(current_subsystem_))
        current_message_.append(std::to_string(value));
    return *this;
}

MessageLogger &MessageLogger::operator<<(size_t value)
{
    if (isMessageTypeEnabled(current_type_) && isSubsystemEnabled(current_subsystem_))
        current_message_.append(std::to_string(value));
    return *this;
}

MessageLogger &MessageLogger::operator<<(double value)
{
    if (isMessageTypeEnabled(current_type_) && isSubsystemEnabled(current_subsystem_))
        current_message_.append(std::to_string(value));
    return *this;
}

} // namespace misc
} // namespace xero
