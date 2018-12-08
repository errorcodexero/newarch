#pragma once

#include "MessageDestFile.h"
#include <ctime>
#include <array>


/// \file

namespace xero
{
namespace misc
{
/// \brief This class represents a message destination that is a file annotated with the current date and time
class MessageDestDatedFile : public MessageDestFile
{
  public:
  	/// \brief create the message destination file in the given base directory
	/// \param basedir the base directory for the message destination file
	MessageDestDatedFile(const std::string &basedir)
	{
		std::array<char, 64> buffer;
		time_t now;
		tm *ltm;

		time(&now);
		ltm = localtime(&now);

		strftime(&buffer[0], buffer.size(), "logfile-%m-%d-%Y-%H-%M-%S", ltm);

		std::string file = basedir;
		file += "/";
		file += &buffer[0];

		setFile(file);
	}
};
} // namespace base
} // namespace xero
