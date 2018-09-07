#pragma once

#include <string>
#include <map>

namespace xero {
namespace misc {

class MessageLoggerData {
public:
	MessageLoggerData();

	MessageLoggerData(const std::string &label);

	MessageLoggerData &init(const std::string &label);

	MessageLoggerData &add(const std::string &key, const std::string &value);

	MessageLoggerData &add(const std::string &key, const char *value);

	MessageLoggerData &add(const std::string &key, double);

	MessageLoggerData &add(const std::string &key, int);

	MessageLoggerData &add(const std::string &key, size_t);

	MessageLoggerData &add(const std::string &key, bool);

	std::string toString();

private:
	std::string label_;
	std::map<std::string, std::string> data_;
};

} // namespace misc
} // namespace xero
