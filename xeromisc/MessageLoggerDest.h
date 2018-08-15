#pragma once

#include "MessageLogger.h"
#include <cstdint>
#include <string>

namespace xero
{
namespace misc
{

/// \brief A destination for a message logger
class MessageLoggerDest
{
public:
	/// \brief write the given message to the destination
	/// \param type the type of the message
	/// \param subs the subsystems the message belongs to
	/// \param msg the message to write
    virtual void displayMessage(const MessageLogger::MessageType &type, uint64_t subs, const std::string &msg) = 0;
};

} // namespace misc
} // namespace xero
