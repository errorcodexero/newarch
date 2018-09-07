#include "MessageLoggerData.h"

#include <sstream>

namespace xero {
namespace misc {

MessageLoggerData::MessageLoggerData() {}

MessageLoggerData::MessageLoggerData(const std::string &label) {
	init(label);
}

MessageLoggerData &MessageLoggerData::init(const std::string &label) {
	label_ = label;
	data_.clear();
	return *this;
}

MessageLoggerData &MessageLoggerData::add(const std::string &key, const std::string &value) {
	data_[key] = value;
	return *this;
}

MessageLoggerData &MessageLoggerData::add(const std::string &key, const char *value) {
	add(key, std::string(value));
	return *this;
}

MessageLoggerData &MessageLoggerData::add(const std::string &key, double value) {
	add(key, std::to_string(value));
	return *this;
}

MessageLoggerData &MessageLoggerData::add(const std::string &key, int value) {
	add(key, std::to_string(value));
	return *this;
}

MessageLoggerData &MessageLoggerData::add(const std::string &key, size_t value) {
	add(key, std::to_string(value));
	return *this;
}

MessageLoggerData &MessageLoggerData::add(const std::string &key, bool value) {
	add(key, value ? "true" : "false");
	return *this;
}

std::string MessageLoggerData::toString() {
	std::stringstream ss;
	ss << label_ << " - ";
	bool first = true;
	for (const auto &kv_pair : data_) {
		if (!first) {
			ss << ", ";
		} else {
			first = false;
		}
		ss << kv_pair.first << ":" << kv_pair.second;
	}
	return ss.str();
}

} // namespace misc
} // namespace xero
