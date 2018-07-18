#pragma once

#include "message_logger_dest.h"
#include <ostream>
#include <string>

class messageDestStream : public messageLoggerDest
{
public:
    messageDestStream(std::ostream &strm) : m_stream(strm)
    {
    }
    
    virtual ~messageDestStream()
    {
	m_stream << std::flush ;
    }

    virtual void displayMessage(const messageLogger::messageType &type, uint64_t subs, const std::string &msg)
    {
	m_stream << msg << std::endl ;
    }

private:
    std::ostream &m_stream ;
} ;
