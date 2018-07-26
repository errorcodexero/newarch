#include "messageLogger.h"
#include "messageLoggerDest.h"
#include <sstream>
#include <iostream>

namespace xero
{
namespace misc
{
messageLogger::messageLogger()
{
    //Initialize maps
    mCurrentMessage = "";
    mInMessage = false;
    mSubsystemsEnabled = 0;
}

void messageLogger::enableType(const messageType &type)
{
    mEnabledModes.push_back(type);
}

void messageLogger::disableType(const messageType &type)
{
    auto it = std::find(mEnabledModes.begin(), mEnabledModes.end(), type);
    if (it != mEnabledModes.end())
        mEnabledModes.erase(it);
}

void messageLogger::enableSubsystem(uint64_t sys)
{
    mSubsystemsEnabled |= sys;
}

void messageLogger::disableSubsystem(uint64_t sys)
{
    mSubsystemsEnabled &= ~sys;
}

bool messageLogger::isMessageTypeEnabled(const messageType &type)
{
    auto it = std::find(mEnabledModes.begin(), mEnabledModes.end(), type);
    return it != mEnabledModes.end();
}

bool messageLogger::isSubsystemEnabled(uint64_t subs)
{
    if (subs == 0)
        return true;

    return (mSubsystemsEnabled & subs) != 0;
}

void messageLogger::startMessage(const messageType &type)
{
    if (mInMessage)
    {
        mCurrentMessage += "FAILED TO CALL ENDMESSAGE";
        endMessage();
    }
    mCurrentType = type;
    mInMessage = true;
    mCurrentSubsystem = 0;
}

void messageLogger::startMessage(const messageType &type, uint64_t sub)
{
    if (mInMessage)
    {
        mCurrentMessage += "FAILED TO CALL ENDMESSAGE - ";
        endMessage();
    }

    mCurrentType = type;
    mInMessage = true;
    mCurrentSubsystem = sub;
}

void messageLogger::endMessage()
{
    mInMessage = false;
    if (isMessageTypeEnabled(mCurrentType) && isSubsystemEnabled(mCurrentSubsystem))
    {
        for (auto dest_p : m_destinations)
            dest_p->displayMessage(mCurrentType, mCurrentSubsystem, mCurrentMessage);
    }
    mCurrentMessage = "";
}

messageLogger &messageLogger::operator<<(const std::string &value)
{
    if (isMessageTypeEnabled(mCurrentType) && isSubsystemEnabled(mCurrentSubsystem))
        mCurrentMessage.append(value);
    return *this;
}

messageLogger &messageLogger::operator<<(const char *value_p)
{
    if (isMessageTypeEnabled(mCurrentType) && isSubsystemEnabled(mCurrentSubsystem))
        mCurrentMessage.append(std::string(value_p));
    return *this;
}

messageLogger &messageLogger::operator<<(int value)
{
    if (isMessageTypeEnabled(mCurrentType) && isSubsystemEnabled(mCurrentSubsystem))
        mCurrentMessage.append(std::to_string(value));
    return *this;
}

messageLogger &messageLogger::operator<<(size_t value)
{
    if (isMessageTypeEnabled(mCurrentType) && isSubsystemEnabled(mCurrentSubsystem))
        mCurrentMessage.append(std::to_string(value));
    return *this;
}

messageLogger &messageLogger::operator<<(double value)
{
    if (isMessageTypeEnabled(mCurrentType) && isSubsystemEnabled(mCurrentSubsystem))
        mCurrentMessage.append(std::to_string(value));
    return *this;
}
} // namespace misc
} // namespace xero
