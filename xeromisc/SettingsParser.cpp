#include "SettingsParser.h"
#include <sstream>
#include <fstream>
#include <cctype>

namespace xero {
namespace misc {

const std::string SettingsParser::var_prefix_("var:") ;

SettingsParser::SettingsParser(MessageLogger &logger, uint64_t msggroup) : logger_(logger) {
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
			if (key.length() == 0 && value.length() == 0) {
				//
				// This is a comment only line, it is ok
				//
				return true ;
			}

			// If the key hasn't been found, the line is invalid
			if(key.length() == 0) {
				logger_.startMessage(MessageLogger::MessageType::warning, msggroup_);
				logger_ << filename << ": line " << line_num << ": Comment started without finding key";
				logger_.endMessage();
				return false;
			}

			// If the key has been found but not the value, the line is invalid
			else if(value.length() == 0) {
				logger_.startMessage(MessageLogger::MessageType::warning, msggroup_);
				logger_ << filename << ": line " << line_num << ": Comment started without finding value";
				logger_.endMessage();
				return false;
			}

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
		logger_.startMessage(MessageLogger::MessageType::warning, msggroup_);
		logger_ << filename << ": line " << line_num << ": Unterminated string";
		logger_.endMessage();
		return false;
	}

	if (key.length() == 0 && value.length() == 0) {
		//
		// This is an empty line
		//
		return true ;
	}

	// If the key hasn't been found, the line is invalid
	if(key.length() == 0) {
		logger_.startMessage(MessageLogger::MessageType::warning, msggroup_);
		logger_ << filename << ": line " << line_num << ": Line ended without finding key";
		logger_.endMessage();
		return false;
	}

	// Make the value what was found up to the end of the line
	value = buffer.str();

	// If there was nothing between the key and the end of the line, the line is invalid
	if(value.length() == 0) {
		logger_.startMessage(MessageLogger::MessageType::warning, msggroup_);
		logger_ << filename << ": line " << line_num << ": Line ended without finding value";
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
				logger_.startMessage(MessageLogger::MessageType::warning, msggroup_);
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
		if (value[0] == '0' && (value[1] == 'X' || value[1] == 'x')) {
			//
			// This is a hex value
			//
			std::string sub = value.substr(2) ;
			result = std::stoi(sub, &output, 16) ;
			if (output != sub.length())
				return false ;
		}
		else {
			result = std::stoi(value, &output);

			// Check for extra characters in value
			if(output != value.length())
				return false;
		}

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

bool SettingsParser::isDefined(const std::string &key) const {
	return settings_.find(key) != settings_.end();
}

bool SettingsParser::isDefinedOnGet(const std::string &key, const std::string &type) const {
	if(!isDefined(key)) {
		std::cerr << "Missing key '" << key << "'" << std::endl ;
		logger_.startMessage(MessageLogger::MessageType::error, msggroup_);
		logger_ << "Tried to get " << type << " at " << key << " but no such value is defined.";
		logger_.endMessage();
		return false;
	}
	return true;
}

Setting &SettingsParser::get(const std::string &key) {
	auto it = settings_.find(key) ;
	assert(it != settings_.end()) ;

	return it->second ;	
}

void SettingsParser::set(const std::string &key, bool value) {
	if (key.length() > var_prefix_.length() && key.substr(0, var_prefix_.length()) == var_prefix_) {
		logger_.startMessage(MessageLogger::MessageType::debug, msggroup_) ;
		logger_ << "Variable '" << key << "' set to value " ;
		logger_ << (value ? "true" : "false") ;
		logger_.endMessage() ;
	}

	settings_[key] = Setting(value);
}

void SettingsParser::set(const std::string &key, int value) {
	if (key.length() > var_prefix_.length() && key.substr(0, var_prefix_.length()) == var_prefix_) {
		logger_.startMessage(MessageLogger::MessageType::debug, msggroup_) ;
		logger_ << "Variable '" << key << "' set to value " ;
		logger_ << value ;
		logger_.endMessage() ;
	}	
	settings_[key] = Setting(value);
}

void SettingsParser::set(const std::string &key, double value) {
	if (key.length() > var_prefix_.length() && key.substr(0, var_prefix_.length()) == var_prefix_) {
		logger_.startMessage(MessageLogger::MessageType::debug, msggroup_) ;
		logger_ << "Variable '" << key << "' set to value " ;
		logger_ << value ;
		logger_.endMessage() ;
	}		
	settings_[key] = Setting(value);
}

void SettingsParser::set(const std::string &key, const std::string &value) {
	if (key.length() > var_prefix_.length() && key.substr(0, var_prefix_.length()) == var_prefix_) {
		logger_.startMessage(MessageLogger::MessageType::debug, msggroup_) ;
		logger_ << "Variable '" << key << "' set to value " ;
		logger_ << value ;
		logger_.endMessage() ;
	}	
	settings_[key] = Setting(value);
}

bool SettingsParser::getBoolean(const std::string &key) const {
	return getSetting(key, "boolean").getBoolean();
}

bool SettingsParser::getBoolean(const std::string &key, const bool &default_value) const {
	return getSetting(key, Setting(default_value), "boolean").getBoolean();
}

int SettingsParser::getInteger(const std::string &key) const {
	return getSetting(key, "integer").getInteger();
}

int SettingsParser::getInteger(const std::string &key, const int &default_value) const {
	return getSetting(key, Setting(default_value), "integer").getInteger();
}

double SettingsParser::getDouble(const std::string &key) const {
	return getSetting(key, "double").getDouble();
}

double SettingsParser::getDouble(const std::string &key, const double &default_value) const {
	return getSetting(key, Setting(default_value), "double").getDouble();
}

const std::string &SettingsParser::getString(const std::string &key) const {
	return getSetting(key, "string").getString();
}

const std::string &SettingsParser::getString(const std::string &key, const std::string &default_value) const {
	return getSetting(key, Setting(default_value), "string").getString();
}

const Setting &SettingsParser::getSetting(const std::string &key, const std::string &type) const {
	assert(isDefinedOnGet(key, type));
	return settings_.find(key)->second;
}

const Setting &SettingsParser::getSetting(const std::string &key, const Setting &default_value, const std::string &type) const {
	return isDefinedOnGet(key, type) ? settings_.find(key)->second : default_value;
}

} // namespace misc
} // namespace xero
