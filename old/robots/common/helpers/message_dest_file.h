#pragma once

#include "message_logger_dest.h"
#include <fstream>
#include <string>
#include <chrono>



class messageDestFile : public messageLoggerDest
{
public:
	messageDestFile();
	messageDestFile(const std::string &name);
	virtual ~messageDestFile();
	void setTimeout(unsigned long int timeout_limit);
	bool setFile(const std::string &name);
	virtual void displayMessage(const messageLogger::messageType &type, uint64_t subs, const std::string &msg);

private:
    void initialize();
    std::ofstream *m_strm_p ;
	std::list<std::string> m_msg_q;
	std::string m_filename;
	bool m_enabled = true , m_ref_established = false;
	std::chrono::time_point<std::chrono::steady_clock> start; //Timeout reference point (first file open failure)
	int m_timeout_limit;
} ;
