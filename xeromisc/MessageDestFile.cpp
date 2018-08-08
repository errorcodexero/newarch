#include "MessageDestFile.h"

namespace xero
{
namespace misc
{

void MessageDestFile::initialize()
{
	strm_p_ = nullptr;
	setTimeout(15 * 1000); // Default to 15 seconds.  Stored in ms.
}

MessageDestFile::MessageDestFile()
{
	initialize();
}

MessageDestFile::MessageDestFile(const std::string &name)
{
	initialize();
	setFile(name);
}

MessageDestFile::~MessageDestFile()
{
	if (strm_p_ != nullptr)
		delete strm_p_;
}

void MessageDestFile::setTimeout(unsigned long int timeout_limit)
{
	timeout_limit_ = timeout_limit;
}

bool MessageDestFile::setFile(const std::string &name)
{
	filename_ = name;
	strm_p_ = new std::ofstream(name);
	if (!strm_p_->is_open())
	{
		if (!ref_established_)
		{
			start_ = std::chrono::steady_clock::now();
			ref_established_ = true;
			std::cout << "Warning: Could not open log file '" << name << "' for writing. Will keep trying to reopen it." << std::endl;
		}
		strm_p_ = nullptr;
		return false;
	}
	return true;
}

void MessageDestFile::displayMessage(const MessageLogger::MessageType &type, uint64_t subs, const std::string &msg)
{
	if (!enabled_)
	{
		return;
	}

	std::string prefix;
	if (type == MessageLogger::MessageType::warning)
		prefix == "WARNING: ";
	if (type == MessageLogger::MessageType::error)
		prefix == "ERROR: ";
	std::string appended_msg = prefix + msg;
	if (strm_p_ != nullptr)
	{
		(*strm_p_) << (appended_msg) << std::endl;
	}
	else
	{
		if (setFile(filename_))
		{ // Succeeded in opening file
			std::cout << "Succeeded in opening log file." << std::endl;
			for (auto const &m : msg_q_)
			{
				(*strm_p_) << m << std::endl;
			}
			msg_q_.clear();
			(*strm_p_) << (appended_msg) << std::endl;
		}
		else
		{
			msg_q_.push_back(appended_msg);
			auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start_).count();
			if (elapsed > timeout_limit_)
			{
				std::cout << "Logging disabled. Timeout reached and log file still not successfully opened." << std::endl;
				enabled_ = false;
			}
		};
	}
}

} // namespace misc
} // namespace xero
