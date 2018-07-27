#pragma once

#include "messageLoggerDest.h"
#include <fstream>
#include <string>
#include <chrono>
#include <sys/types.h>
#include <dirent.h>

namespace xero
{
namespace misc
{

class messageDestSeqFile : public messageLoggerDest
{
  public:
	messageDestSeqFile(const std::string &dir, const std::string &filename, unsigned long int timeout = 15000);
	virtual ~messageDestSeqFile();
	virtual void displayMessage(const messageLogger::messageType &type, uint64_t subs, const std::string &msg);

  private:
	bool openfile();
	std::string getFileName(DIR *dir_p);

  private:
	std::ofstream *m_strm_p;
	std::list<std::string> m_msg_q;
	std::string m_filename;
	std::string m_dirname;
	bool m_enabled;
	bool m_is_open;
	bool m_ref_established;
	std::chrono::time_point<std::chrono::steady_clock> start;
	int m_timeout_limit;
};
} // namespace misc
} // namespace xero
