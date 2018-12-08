#pragma once

#include "MessageLoggerDest.h"
#include "DriverStation.h"


/// \file

namespace xero
{
namespace misc
{

/// \brief this class is a message logger destination that sends errors and warnings to the driver station
/// \see MessageLogger
class MessageDestDS : public MessageLoggerDest
{
  public:
	/// \brief create a new driver station message logger destination
	MessageDestDS(){};

	/// \brief destroy a driver station message logger
	virtual ~MessageDestDS(){};

	/// \brief display a message to the driver station if it is an error or warning message
	/// \param type the type of the message
	/// \param subs the subsystems the message belongs to
	/// \param msg the message to display
	void displayMessage(const MessageLogger::MessageType &type, uint64_t subs, const std::string &msg)
	{
		frc::DriverStation &ds = frc::DriverStation::GetInstance();
		if (type == MessageLogger::MessageType::error)
			ds.ReportError(msg.c_str());
		else if (type == MessageLogger::MessageType::warning)
			ds.ReportError(msg.c_str());
	};
};

} // namespace misc
} // namespace xero
