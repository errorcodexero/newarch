#pragma once

#include <iostream>
#include <string>
#include <list>
#include <algorithm>
#include <cassert>
#include <memory>

#include "MessageLoggerData.h"


/// \file

namespace xero
{
namespace misc
{

class MessageLoggerDest;

/// \brief A utility for logging messages
class MessageLogger
{
  public:
    /// \brief the type of a message being logged
    enum class MessageType
    {
        debug,   ///< the message is a debug message
        info,    ///< the message is informational
        warning, ///< the message is a warning
        error    ///< the message is an error message
    };

  public:
    /// \brief create a new message logger object
    MessageLogger();

    /// \brief clear all message destinations
    void clear() ;

    /// \brief returns true if a given message type is active
    /// \param type the type of message to check for active
    /// \returns true if the message type is active, otherwise false
    bool isMessageTypeEnabled(const MessageType &type);

    /// \brief returns true if a given subsystem is enabled
    /// \param sub the subsystem to check on
    /// \returns true if a sbusystem is enabled
    bool isSubsystemEnabled(uint64_t sub);

    /// \brief enable a given message type
    /// \param type the type of message to enable
    void enableType(const MessageType &type);

    /// \brief disable a given message type
    /// \param type the type of message to disable
    void disableType(const MessageType &type);

    /// \brief enable a given message type
    /// \param subsystem the subsystem to enable
    void enableSubsystem(uint64_t subsystem);

    /// \brief disable a given message type
    /// \param subsystem the subsystem to disable
    void disableSubsystem(uint64_t subsystem);

    /// \brief start a new message
    /// \param type type the type of message to start
    void startMessage(const MessageType &type);

    /// \brief start a new message
    /// \param type the type of message to start
    /// \param subsystem the subsystem the message is about
    void startMessage(const MessageType &type, uint64_t subsystem);

    /// \brief end the current message
    void endMessage();

    /// \brief operator overload the output a string value
    /// \param value_p the string to output
    /// \returns a copy of the message logger
    MessageLogger &operator<<(const char *value_p);

    /// \brief operator overload the output a string value
    /// \param value the string to output
    /// \returns a copy of the message logger
    MessageLogger &operator<<(const std::string &value);

    /// \brief operator overload the output an integer value
    /// \param value the integer to output
    /// \returns a copy of the message logger
    MessageLogger &operator<<(int value);

    /// \brief operator overload the output an integer value
    /// \param value the integer to output
    /// \returns a copy of the message logger
    MessageLogger &operator<<(size_t value);

    /// \brief operator overload the output a floating point value
    /// \param value the floating point number to output
    /// \returns a copy of the message logger
    MessageLogger &operator<<(double value);

    /// \brief start logging a block of data
    /// \param label the label for the data
    /// \returns a reference to the MessageLogger object
    MessageLogger &startData(const std::string &label) {
        current_data_.init(label);
        return *this;
    }

    /// \brief add data to the message logger
    /// \param key the name of the data
    /// \param value the data to log
    /// \returns a reference to the MessageLogger object
    MessageLogger &addData(const std::string &key, const std::string &value) {
        current_data_.add(key, value);
        return *this;
    }

    /// \brief add data to the message logger
    /// \param key the name of the data
    /// \param value the data to log
    /// \returns a reference to the MessageLogger object
    MessageLogger &addData(const std::string &key, const char *value) {
        current_data_.add(key, value);
        return *this;
    }

    /// \brief add data to the message logger
    /// \param key the name of the data
    /// \param value the data to log
    /// \returns a reference to the MessageLogger object
    MessageLogger &addData(const std::string &key, double value) {
        current_data_.add(key, value);
        return *this;
    }

    /// \brief add data to the message logger
    /// \param key the name of the data
    /// \param value the data to log
    /// \returns a reference to the MessageLogger object
    MessageLogger &addData(const std::string &key, int value) {
        current_data_.add(key, value);
        return *this;
    }

    /// \brief add data to the message logger
    /// \param key the name of the data
    /// \param value the data to log
    /// \returns a reference to the MessageLogger object
    MessageLogger &addData(const std::string &key, size_t value) {
        current_data_.add(key, value);
        return *this;
    }

    /// \brief add data to the message logger
    /// \param key the name of the data
    /// \param value the data to log
    /// \returns a reference to the MessageLogger object
    MessageLogger &addData(const std::string &key, bool value) {
        current_data_.add(key, value);
        return *this;
    }

    /// \brief end the data being sent to the logger
    void endData() {
        *this << current_data_.toString();
    }

    /// \brief add a new destiation for messages
    /// \param dest_p the new destination to add
    void addDestination(std::shared_ptr<MessageLoggerDest> dest_p)
    {
        destinations_.push_back(dest_p);
    }

    /// \brief Remove matching destinations.
    /// \param dest_p the destination to remove
    void removeDestination(std::shared_ptr<MessageLoggerDest> dest_p)
    {
        destinations_.remove(dest_p);
    }

  private:
    // The modes currently enabled
    std::list<MessageType> enabled_modes_;

    // The subsystems enabled, or zero if all are enabled
    uint64_t subsystems_enabled_;

    // If true, we have seen a startMessage() call but not an
    // endMessage() call
    bool in_message_;

    // The current message type
    MessageType current_type_;

    // The current message subsystem
    uint64_t current_subsystem_;

    // The current message
    std::string current_message_;

    MessageLoggerData current_data_;

    // The list of message logger destinations
    std::list<std::shared_ptr<MessageLoggerDest>> destinations_;
};

} // namespace misc
} // namespace xero
