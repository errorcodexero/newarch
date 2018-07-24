#pragma once

#include "messageLogger.h"
#include <cstdint>
#include <string>

namespace xero
{
namespace misc
{
class messageLoggerDest
{
  public:
    virtual void displayMessage(const messageLogger::messageType &type, uint64_t subs, const std::string &msg) = 0;
};
} // namespace misc
} // namespace xero
