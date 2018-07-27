#pragma once

#include <iostream>
#include <string>
#include <list>
#include <algorithm>
#include <cassert>
#include <memory>

namespace xero
{
namespace misc
{

class messageLoggerDest;

class messageLogger
{
  public:
    /// \brief the type of a message being logged
    enum class messageType
    {
        debug,   ///< the message is a debug message
        info,    ///< the message is informational
        warning, ///< the message is a warning
        error    ///< the message is an error message
    };

  public:
    /// \brief create a new message logger object
    messageLogger();

    /// \brief returns true if a given message type is active
    /// \param the type of message to check for active
    /// \returns true if the message type is active, otherwise false
    bool isMessageTypeEnabled(const messageType &type);

    /// \brief returns true if a given subsystem is enabled
    /// \returns true if a sbusystem is enabled
    bool isSubsystemEnabled(uint64_t sub);

    /// \brief enable a given message type
    /// \param type the type of message to enable
    void enableType(const messageType &type);

    /// \brief disable a given message type
    /// \param type the type of message to disable
    void disableType(const messageType &type);

    /// \brief enable a given message type
    /// \param the subsystem to enable
    void enableSubsystem(uint64_t subsystem);

    /// \brief disable a given message type
    /// \param the subsystem to disable
    void disableSubsystem(uint64_t subsystem);

    /// \brief start a new message
    /// \param type the type of message to start
    void startMessage(const messageType &type);

    /// \brief start a new message
    /// \param type the type of message to start
    /// \param the subsystem the message is about
    void startMessage(const messageType &type, uint64_t subsystem);

    /// \brief end the current message
    void endMessage();

    /// \brief operator overload the output a string value
    /// \param value_p the string to output
    /// \returns a copy of the message logger
    messageLogger &operator<<(const char *value_p);

    /// \brief operator overload the output a string value
    /// \param value the string to output
    /// \returns a copy of the message logger
    messageLogger &operator<<(const std::string &value);

    /// \brief operator overload the output an integer value
    /// \param value the integer to output
    /// \returns a copy of the message logger
    messageLogger &operator<<(int value);

    /// \brief operator overload the output an integer value
    /// \param value the integer to output
    /// \returns a copy of the message logger
    messageLogger &operator<<(size_t value);

    /// \brief operator overload the output a floating point value
    /// \param value the floating point number to output
    /// \returns a copy of the message logger
    messageLogger &operator<<(double value);

    /// \brief add a new destiation for messages
    /// \param dest_p the new destination to add
    void addDestination(std::shared_ptr<messageLoggerDest> dest_p)
    {
        m_destinations.push_back(dest_p);
    }

    /// \brief Remove matching destinations.
    /// \param dest_p the destination to remove
    void removeDestination(std::shared_ptr<messageLoggerDest> dest_p)
    {
        m_destinations.remove(dest_p);
    }

  private:
    // The modes currently enabled
    std::list<messageType> mEnabledModes;

    // The subsystems enabled, or zero if all are enabled
    uint64_t mSubsystemsEnabled;

    // If true, we have seen a startMessage() call but not an
    // endMessage() call
    bool mInMessage;

    // The current message type
    messageType mCurrentType;

    // The current message subsystem
    uint64_t mCurrentSubsystem;

    // The current message
    std::string mCurrentMessage;

    // The list of message logger destinations
    std::list<std::shared_ptr<messageLoggerDest>> m_destinations;
};
} // namespace misc
} // namespace xero
