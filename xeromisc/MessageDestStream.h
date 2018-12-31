#pragma once

#include "MessageLoggerDest.h"
#include <ostream>
#include <string>


/// \file

namespace xero
{
namespace misc
{

/// \brief A stream destination for a message logger
class MessageDestStream : public MessageLoggerDest
{
public:
    /// \brief create a new message logger stream destination
    /// \param strm the stream to use as a message destination
    MessageDestStream(std::ostream &strm) : stream_(strm)
    {
    }

    /// \brief destroy the message logger stream destination
    virtual ~MessageDestStream()
    {
        stream_ << std::flush;
    }

    /// \brief write the given message to the stream
    /// \param type the type of the message
    /// \param subs the subsystems the message belongs to
    /// \param msg the message to write
    virtual void displayMessage(const MessageLogger::MessageType &type, uint64_t subs, const std::string &msg)
    {
        stream_ << msg << std::endl;
    }

private:
    std::ostream &stream_;
};

} // namespace misc
} // namespace xero
