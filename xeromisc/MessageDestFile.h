#pragma once

#include "MessageLoggerDest.h"
#include <fstream>
#include <string>
#include <chrono>


/// \file

namespace xero
{
namespace misc
{

/// \brief A file destination for a message logger
class MessageDestFile : public MessageLoggerDest
{
public:
	/// \brief create a new message logger file destination
	MessageDestFile();

	/// \brief create a new message logger file destination using the given filename for the file
	/// \param name the filename of the file to use as a message destination
	MessageDestFile(const std::string &name);

	/// \brief destroy the message logger file destination
	virtual ~MessageDestFile();

	/// \brief set the amount of time in milliseconds after which to stop trying to open the file
	/// \param timeout_limit the amount of time in milliseconds after which to stop trying to open the file
	void setTimeout(unsigned long int timeout_limit);

	/// \brief open the file with the given filename to use as the message logger destination file
	/// \param name the filename of the file to use as a message destination
	/// \returns true if the file was successfully opened
	bool setFile(const std::string &name);

	/// \brief write the given message to the file
	/// \param type the type of the message
	/// \param subs the subsystems the message belongs to
	/// \param msg the message to write
	virtual void displayMessage(const MessageLogger::MessageType &type, uint64_t subs, const std::string &msg);

private:
	void initialize();
	std::ofstream *strm_p_;
	std::list<std::string> msg_q_;
	std::string filename_;
	bool enabled_ = true, ref_established_ = false;
	std::chrono::time_point<std::chrono::steady_clock> start_; //Timeout reference point (first file open failure)
	int timeout_limit_;
};

} // namespace misc
} // namespace xero
