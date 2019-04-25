#pragma once

#include "message_logger.h"
#include <cstdint>
#include <string>

class messageLoggerDest
{
public:
    virtual void displayMessage(const messageLogger::messageType &type, uint64_t subs, const std::string &msg) = 0 ;
} ;
