#include "SettingsParser.h"
#include <sstream>
#include <fstream>

using namespace xero::misc;

SettingsParser::SettingsParser(messageLogger &logger, uint64_t msggroup) : logger_(logger) {
	msggroup_ = msggroup;
}

SettingsParser::~SettingsParser() {
	settings_.clear();
}

bool SettingsParser::readLine(const std::string &line, std::string &key, std::string &value, bool &is_string, const std::string& filename, int line_num) {
	std::stringstream buffer;
	bool in_string = false;

	for(unsigned i = 0; i < line.length(); i++) {
		// Check for comment start
		if(line[i] == '#') {
			// If the key hasn't been found, the line is invalid
			if(key.length() == 0)
				return false;
			// If the key has been found but not the value, the line is invalid
			else if(value.length() == 0)
				return false;

			// Otherwise, set the value to what's come so far and finish
			value = buffer.str();
			return true;
		// Check for string start
		} else if(line[i] == '"') {
			in_string = !in_string;
			if(key.length() > 0)
				is_string = true;
			continue;
		// Check for separating space if not in a string
		} else if(!in_string && std::isspace(line[i])) {
			if(key.length() == 0) {
				key = buffer.str();
				buffer.str("");
				continue;
			} else if (value.length() > 0) {
				value = buffer.str();
				return true;
			}
		// If the character is none of these, add it to the buffer
		} else {
			buffer.put(line[i]);
		}
	}

	// Check for unterminated string
	if(in_string) {
		logger_.startMessage(messageLogger::messageType::warning, msggroup_);
		logger_ << filename << ": line " << line_num << ": Unterminated string";
		logger_.endMessage();
		return false;
	}

	return true;
}

bool SettingsParser::readFile(const std::string &filename) {
	std::ifstream file(filename);
	if(file.bad() || file.fail()) {
		return false;
	}

	int line_num = 0;

	bool bool_output;
	int int_output;
	double double_output;
	std::string string_output;

	std::string line;
	while(std::getline(file, line)) {
		std::string key, value;
		bool is_string = false;

		line_num++;

		if(readLine(line, key, value, is_string, filename, line_num) && key.length() > 0 && value.length() > 0) {
			if(parseBoolean(value, bool_output))
				set(key, bool_output);
			else if(parseInteger(value, int_output))
				set(key, int_output);
			else if(parseDouble(value, double_output))
				set(key, double_output);
			else if(is_string && parseString(value, string_output))
				set(key, string_output);
			else {
				logger_.startMessage(messageLogger::messageType::warning, msggroup_);
				logger_ << filename << ": " << line_num << ": Unable to parse value '" << value << "'";
				logger_.endMessage();
			}
		}
	}

	return true;
}

bool SettingsParser::parseBoolean(const std::string &value, bool &result) {
	if(value == "true") {
		result = true;
		return true;
	} else if(value == "false") {
		result = false;
		return true;
	}

	return false;
}

bool SettingsParser::parseInteger(const std::string &value, int &result) {
	size_t output;

	try {
		result = std::stoi(value, &output);

		// Check for extra characters in value
		if(output != value.length())
			return false;
	} catch(...) {
		return false;
	}

	return true;
}

bool SettingsParser::parseDouble(const std::string &value, double &result) {
	size_t output;

	try {
		result = std::stod(value, &output);

		// Check for extra characters in value
		if(output != value.length())
			return false;
	} catch(...) {
		return false;
	}

	return true;
}

bool SettingsParser::parseString(const std::string &value, std::string &result) {
	if(value.length() < 2)
		return false;

	result = value;

	return true;
}

bool SettingsParser::isDefined(const std::string &key) {
	return settings_.find(key) != settings_.end();
}

void SettingsParser::set(const std::string &key, bool value) {
	settings_[key] = Setting(value);
}

void SettingsParser::set(const std::string &key, int value) {
	settings_[key] = Setting(value);
}

void SettingsParser::set(const std::string &key, double value) {
	settings_[key] = Setting(value);
}

void SettingsParser::set(const std::string &key, const std::string &value) {
	settings_[key] = Setting(value);
}

bool SettingsParser::getBoolean(const std::string &key) {
	return settings_[key].getBoolean();
}

int SettingsParser::getInteger(const std::string &key) {
	return settings_[key].getInteger();
}

double SettingsParser::getDouble(const std::string &key) {
	return settings_[key].getDouble();
}

std::string SettingsParser::getString(const std::string &key) {
	return settings_[key].getString();
}
