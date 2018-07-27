#pragma once

#include "messageLoggerDest.h"
#include "DriverStation.h"

/// \brief this class is a message logger destination that sends errors and warnings to the driver station
/// \see messageLogger

namespace xero
{
namespace misc
{
class messageDestDS : public messageLoggerDest
{
  public:
	/// \brief create a new driver station message logger destination
	messageDestDS(){};
	/// \brief destroy a driver station message logger
	virtual ~messageDestDS(){};

	/// \brief display a message to the driver station if it is an error or warning message
	void displayMessage(const messageLogger::messageType &type, uint64_t subs, const std::string &msg)
	{
		frc::DriverStation &ds = DriverStation::GetInstance();
		if (type == messageLogger::messageType::error)
			ds.ReportError(msg);
		else if (type == messageLogger::messageType::warning)
			ds.ReportError(msg);
	};
};
} // namespace misc
} // namespace xero
